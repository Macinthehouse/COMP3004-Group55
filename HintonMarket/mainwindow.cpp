#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QStackedWidget>

#include "StartupUI.h"
#include "VendorDashboardUI.h"
#include "MarketScheduleUI.h"

#include "BookingDialog.h"
#include "WaitlistDialog.h"

// Backend
#include "../InMemoryStorageManager.h"
#include "../LoginController.h"
#include "../DashboardController.h"
#include "../WaitlistController.h"
#include "../BookingController.h"
#include "../Vendor.h"
#include "../MarketOperator.h"
#include "../SystemAdministrator.h"
#include "MarketOperatorDashboardUI.h"
#include "../MarketOperator.h"
#include "OperatorBookingDialog.h"
#include "../MarketOperatorController.h"
#include "OperatorCancellationDialog.h"
#include "OperatorWaitlistRemovalDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Backend setup
    m_storage = new InMemoryStorageManager();
    if (!m_storage->loadFromDatabase()) {
        QMessageBox::critical(this, tr("Startup Error"),
                              tr("Failed to load data from SQLite into memory."));
    }

    m_waitlistController = new WaitlistController(*m_storage);
    m_bookingController  = new BookingController(*m_storage, *m_waitlistController);
    m_operatorController = new MarketOperatorController(*m_storage, *m_bookingController, *m_waitlistController);
    m_dashboardController = new DashboardController(*m_storage);
    m_loginController = new LoginController(m_storage);

    // UI setup
    m_stack = new QStackedWidget(this);
    setCentralWidget(m_stack);

    m_startup = new StartupUI(this);
    m_dashboard = new VendorDashboardUI(this);
    m_schedule = new MarketScheduleUI(this);
    m_operatorDashboard = new MarketOperatorDashboardUI(this);

    m_stack->addWidget(m_startup);
    m_stack->addWidget(m_dashboard);
    m_stack->addWidget(m_schedule);
    m_stack->addWidget(m_operatorDashboard);

    // Navigation wiring
    connect(m_startup, &StartupUI::loginRequested, this, [this](const QString& username) {
        const std::string u = username.trimmed().toStdString();
        if (u.empty()) {
            QMessageBox::warning(this, "Login", "Please enter a vendor ID.");
            return;
        }

        User* user = m_loginController->login(u);
        if (!user) {
            QMessageBox::warning(this, tr("Login"), tr("User not found."));
            return;
        }

        // Vendor flow (D1 implemented)
        if (auto* vendor = dynamic_cast<Vendor*>(user)) {
            m_currentUserId = vendor->getId();
            showDashboard();
            return;
        }

        // Non-vendor roles (acknowledge only; D2 scope)
        if (auto* op = dynamic_cast<MarketOperator*>(user)) {
            m_currentUserId = op->getId();
            showOperatorDashboard();
            return;
        }

        if (dynamic_cast<SystemAdministrator*>(user)) {
            QMessageBox::information(
                this,
                tr("Login"),
                tr("Welcome, System Administrator.\n\nSystem Administrator features are not implemented in Deliverable 1.")
            );
            return;
        }

        // Fallback (unknown derived type)
        QMessageBox::information(
            this,
            tr("Login"),
            tr("Welcome.\n\nThis role’s features are not implemented in Deliverable 1.")
        );
    });

    connect(m_dashboard, &VendorDashboardUI::browseMarketDatesRequested, this, [this] {
        showSchedule();
    });

    connect(m_schedule, &MarketScheduleUI::backRequested, this, [this] {
        showDashboard();
    });

    connect(m_dashboard, &VendorDashboardUI::logoutRequested, this, [this] {
        m_currentUserId.clear();
        m_startup->clear();
        showStartup();
    });

    connect(m_dashboard, &VendorDashboardUI::refreshRequested, this, [this] {
        refreshDashboard();
    });

    connect(m_schedule, &MarketScheduleUI::bookRequested, this, [this](const QString& marketDateId) {
        if (m_currentUserId.empty()) return;

        BookingDialog dlg(this);
        dlg.setVendorId(QString::fromStdString(m_currentUserId));
        dlg.setMarketDateIso(marketDateId);
        if (dlg.exec() != QDialog::Accepted) {
            return;
        }

        const auto result = m_bookingController->bookStall(
            m_currentUserId,
            marketDateId.toStdString()
        );

        QMessageBox::information(this, tr("Book Stall"),
                                 QString::fromStdString(result.message));

        refreshDashboard();
        refreshSchedule();
    });

    connect(m_schedule, &MarketScheduleUI::joinWaitlistRequested, this, [this](const QString& marketDateId) {
        if (m_currentUserId.empty()) return;

        WaitlistDialog dlg(this);
        dlg.setMode(WaitlistDialog::Mode::Join);
        dlg.setVendorId(QString::fromStdString(m_currentUserId));
        dlg.setMarketDateIso(marketDateId);
        if (dlg.exec() != QDialog::Accepted) {
            return;
        }

        const auto result = m_waitlistController->joinWaitlist(
            m_currentUserId,
            marketDateId.toStdString()
        );

        QString msg = QString::fromStdString(result.message);
        if (result.queuePosition > 0) {
            msg += tr("\nQueue position: %1").arg(result.queuePosition);
        }

        QMessageBox::information(this, tr("Join Waitlist"), msg);

        refreshDashboard();
        refreshSchedule();
    });

    connect(m_dashboard, &VendorDashboardUI::cancelBookingRequested, this, [this](const QString& marketDateId) {
        if (m_currentUserId.empty()) return;

        const auto choice = QMessageBox::question(
            this,
            tr("Cancel Booking"),
            tr("Cancel booking for %1?").arg(marketDateId),
            QMessageBox::Yes | QMessageBox::No
        );

        if (choice != QMessageBox::Yes) return;

        const auto result = m_bookingController->cancelBooking(
            m_currentUserId,
            marketDateId.toStdString()
        );

        QMessageBox::information(this, tr("Cancel Booking"),
                                 QString::fromStdString(result.message));

        refreshDashboard();
        refreshSchedule();
    });

    connect(m_dashboard, &VendorDashboardUI::leaveWaitlistRequested, this, [this](const QString& marketDateId) {
        if (m_currentUserId.empty()) return;

        WaitlistDialog dlg(this);
        dlg.setMode(WaitlistDialog::Mode::Leave);
        dlg.setVendorId(QString::fromStdString(m_currentUserId));
        dlg.setMarketDateIso(marketDateId);
        if (dlg.exec() != QDialog::Accepted) {
            return;
        }

        const auto result = m_waitlistController->leaveWaitlist(
            m_currentUserId,
            marketDateId.toStdString()
        );

        QMessageBox::information(this, tr("Leave Waitlist"),
                                 QString::fromStdString(result.message));

        refreshDashboard();
        refreshSchedule();
    });

    connect(m_dashboard, &VendorDashboardUI::viewComplianceTextRequested,
            this, [this](const QString& docName, const QString& legalText) {
        QMessageBox::information(this,
                                 tr("Compliance Document: %1").arg(docName),
                                 legalText);
    });

    connect(m_operatorDashboard, &MarketOperatorDashboardUI::logoutRequested, this, [this] {
        m_currentUserId.clear();
        m_startup->clear();
        showStartup();
    });

    connect(m_operatorDashboard, &MarketOperatorDashboardUI::bookForVendorRequested,
            this, [this] {
        OperatorBookingDialog dlg(this);
        if (dlg.exec() != QDialog::Accepted) return;

        const QString vendorId = dlg.vendorId();
        const QString dateId = dlg.marketDateId();

        if (vendorId.isEmpty() || dateId.isEmpty()) {
            QMessageBox::warning(this, tr("Book Stall (Operator)"),
                                 tr("Vendor ID and Market Date are required."));
            return;
        }

        const MarketOperatorResult r =
            m_operatorController->bookForVendor(vendorId.toStdString(), dateId.toStdString());

        QMessageBox::information(this, tr("Book Stall (Operator)"),
                                 QString::fromStdString(r.message));
    });

    connect(m_operatorDashboard, &MarketOperatorDashboardUI::cancelForVendorRequested,
            this, [this] {
        OperatorCancellationDialog dlg(this);
        if (dlg.exec() != QDialog::Accepted) return;

        const QString vendorId = dlg.vendorId();
        const QString dateId = dlg.marketDateId();

        if (vendorId.isEmpty() || dateId.isEmpty()) {
            QMessageBox::warning(this, tr("Cancel Booking (Operator)"),
                                 tr("Vendor ID and Market Date are required."));
            return;
        }

        const MarketOperatorResult r =
            m_operatorController->cancelForVendor(vendorId.toStdString(), dateId.toStdString());

        QMessageBox::information(this, tr("Cancel Booking (Operator)"),
                                 QString::fromStdString(r.message));
    });

    connect(m_operatorDashboard, &MarketOperatorDashboardUI::removeFromWaitlistRequested,
            this, [this] {
        OperatorWaitlistRemovalDialog dlg(this);
        if (dlg.exec() != QDialog::Accepted) return;

        const QString vendorId = dlg.vendorId();
        const QString dateId = dlg.marketDateId();

        if (vendorId.isEmpty() || dateId.isEmpty()) {
            QMessageBox::warning(this, tr("Remove from Waitlist (Operator)"),
                                 tr("Vendor ID and Market Date are required."));
            return;
        }

        const MarketOperatorResult r =
            m_operatorController->removeFromWaitlistForVendor(vendorId.toStdString(), dateId.toStdString());

        QMessageBox::information(this, tr("Remove from Waitlist (Operator)"),
                                 QString::fromStdString(r.message));
    });

    showStartup();
}

MainWindow::~MainWindow()
{
    // Clean up owned backend objects
    delete m_bookingController;
    delete m_waitlistController;
    delete m_dashboardController;
    delete m_loginController;
    delete m_storage;
    delete m_operatorController;

    delete ui;
}

void MainWindow::showStartup()  { m_stack->setCurrentWidget(m_startup); }

void MainWindow::showDashboard() {
    refreshDashboard();
    m_stack->setCurrentWidget(m_dashboard);
}

void MainWindow::showSchedule() {
    refreshSchedule();
    m_stack->setCurrentWidget(m_schedule);
}
void MainWindow::refreshSchedule() {
    const auto dates = m_dashboardController->listMarketDates();
    m_schedule->setSchedule(dates);
}
void MainWindow::refreshDashboard() {
    if (m_currentUserId.empty()) return;

    const VendorDashboardData data = m_dashboardController->getVendorDashboard(m_currentUserId);

    m_dashboard->setDashboardData(data);
}
void MainWindow::showOperatorDashboard() {
    m_operatorDashboard->setOperatorId(QString::fromStdString(m_currentUserId));
    m_stack->setCurrentWidget(m_operatorDashboard);
}
