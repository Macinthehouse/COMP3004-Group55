#include "VendorDashboardUI.h"
#include "../DashboardController.h"
#include "../ComplianceDocument.h"

#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTextBrowser>
#include <QVBoxLayout>

VendorDashboardUI::VendorDashboardUI(QWidget* parent) : QWidget(parent) {
    auto* title = new QLabel(tr("Vendor Dashboard"));
    QFont f = title->font();
    f.setPointSize(f.pointSize() + 3);
    f.setBold(true);
    title->setFont(f);

    m_browseBtn = new QPushButton(tr("Browse Market Dates"));
    m_refreshBtn = new QPushButton(tr("Refresh"));
    m_logoutBtn  = new QPushButton(tr("Logout"));

    auto* topBar = new QHBoxLayout;
    topBar->addWidget(title);
    topBar->addStretch(1);
    topBar->addWidget(m_browseBtn);
    topBar->addWidget(m_refreshBtn);
    topBar->addWidget(m_logoutBtn);

    m_vendorSummaryLabel = new QLabel(tr("Vendor: (not loaded)"));

    // --- Bookings table ---
    m_bookingsTable = new QTableWidget(0, 2);
    m_bookingsTable->setHorizontalHeaderLabels({tr("Market Date"), tr("Category")});
    m_bookingsTable->horizontalHeader()->setStretchLastSection(true);
    m_bookingsTable->verticalHeader()->setVisible(false);
    m_bookingsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_bookingsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_bookingsTable->setSelectionMode(QAbstractItemView::SingleSelection);

    // --- Waitlists table ---
    m_waitlistsTable = new QTableWidget(0, 3);
    m_waitlistsTable->setHorizontalHeaderLabels({tr("Market Date"), tr("Category"), tr("Position")});
    m_waitlistsTable->horizontalHeader()->setStretchLastSection(true);
    m_waitlistsTable->verticalHeader()->setVisible(false);
    m_waitlistsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_waitlistsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_waitlistsTable->setSelectionMode(QAbstractItemView::SingleSelection);

    // --- Compliance Doc ---
    m_complianceTable = new QTableWidget(0, 4);
    m_complianceTable->setHorizontalHeaderLabels({
        tr("Document"),
        tr("Number"),
        tr("Expiry"),
        tr("Valid")
    });
    m_complianceTable->horizontalHeader()->setStretchLastSection(true);
    m_complianceTable->verticalHeader()->setVisible(false);
    m_complianceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_complianceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_complianceTable->setSelectionMode(QAbstractItemView::SingleSelection);

    m_viewComplianceTextBtn = new QPushButton(tr("View Selected Document Text"));

    // --- Notifications ---
    m_notificationsBrowser = new QTextBrowser;
    m_notificationsBrowser->setPlaceholderText(tr("Notifications will appear here."));

    // --- Action buttons ---
    m_cancelBookingBtn = new QPushButton(tr("Cancel Selected Booking"));
    m_leaveWaitlistBtn = new QPushButton(tr("Leave Selected Waitlist"));

    // --- Layout ---
    auto* root = new QVBoxLayout;
    root->addLayout(topBar);
    root->addWidget(m_vendorSummaryLabel);
    root->addSpacing(8);

    // Confirmed Bookings section
    root->addWidget(new QLabel(tr("Confirmed Bookings")));
    root->addWidget(m_bookingsTable);

    auto* bookingActions = new QHBoxLayout;
    bookingActions->addStretch(1);
    bookingActions->addWidget(m_cancelBookingBtn);
    root->addLayout(bookingActions);

    root->addSpacing(8);

    // Active Waitlists section
    root->addWidget(new QLabel(tr("Active Waitlists")));
    root->addWidget(m_waitlistsTable);

    auto* waitlistActions = new QHBoxLayout;
    waitlistActions->addStretch(1);
    waitlistActions->addWidget(m_leaveWaitlistBtn);
    root->addLayout(waitlistActions);

    root->addSpacing(8);

    // Compliance Doc section
    root->addSpacing(8);
    root->addWidget(new QLabel(tr("Compliance Documents")));
    root->addWidget(m_complianceTable);

    auto* compActions = new QHBoxLayout;
    compActions->addStretch(1);
    compActions->addWidget(m_viewComplianceTextBtn);
    root->addLayout(compActions);

    // Notifications section
    root->addWidget(new QLabel(tr("Notifications")));
    root->addWidget(m_notificationsBrowser, 1);

    setLayout(root);

    // --- Navigation connects ---
    connect(m_browseBtn, &QPushButton::clicked, this, &VendorDashboardUI::browseMarketDatesRequested);
    connect(m_refreshBtn, &QPushButton::clicked, this, &VendorDashboardUI::refreshRequested);
    connect(m_logoutBtn,  &QPushButton::clicked, this, &VendorDashboardUI::logoutRequested);

    // --- Action connects ---
    connect(m_cancelBookingBtn, &QPushButton::clicked, this, [this] {
        const QString marketDateId = selectedBookingMarketDateId();
        if (marketDateId.isEmpty()) {
            QMessageBox::warning(this, tr("Cancel Booking"), tr("Please select a booking first."));
            return;
        }
        emit cancelBookingRequested(marketDateId);
    });

    connect(m_leaveWaitlistBtn, &QPushButton::clicked, this, [this] {
        const QString marketDateId = selectedWaitlistMarketDateId();
        if (marketDateId.isEmpty()) {
            QMessageBox::warning(this, tr("Leave Waitlist"), tr("Please select a waitlist entry first."));
            return;
        }
        emit leaveWaitlistRequested(marketDateId);
    });

    // Compliance docs connects
    connect(m_viewComplianceTextBtn, &QPushButton::clicked, this, [this] {
        const int row = m_complianceTable->currentRow();
        if (row < 0) {
            QMessageBox::warning(this, tr("Compliance Documents"),
                                 tr("Please select a compliance document first."));
            return;
        }

        QTableWidgetItem* nameItem = m_complianceTable->item(row, 0);
        if (!nameItem) return;

        const QString docName = nameItem->text();
        const QString legalText = nameItem->data(Qt::UserRole).toString();

        emit viewComplianceTextRequested(docName, legalText);
    });
}

static QString qs(const std::string& s) {
    return QString::fromStdString(s);
}

static QString categoryToQString(VendorCategory c) {
    return (c == VendorCategory::FOOD) ? "Food" : "Artisan";
}

static QString formatBusinessInfo(const VendorSummary& v) {
    return QString(
        "Business Name: %1\n"
        "Owner Name: %2\n"
        "Email: %3\n"
        "Phone: %4\n"
        "Mailing Address: %5\n"
        "Category: %6"
    )
    .arg(qs(v.businessName))
    .arg(qs(v.ownerName))
    .arg(qs(v.email))
    .arg(qs(v.phone))
    .arg(qs(v.mailingAddress))
    .arg(qs(v.category));
}

void VendorDashboardUI::setVendorTitle(const QString& vendorId) {
    m_vendorSummaryLabel->setText(tr("Vendor ID: %1").arg(vendorId));
}

void VendorDashboardUI::setDashboardData(const VendorDashboardData& data) {
    m_vendorSummaryLabel->setText(formatBusinessInfo(data.vendor));
    // ---- Confirmed Bookings ----
    m_bookingsTable->clearContents();
    m_bookingsTable->setRowCount(static_cast<int>(data.confirmedBookings.size()));

    for (int row = 0; row < static_cast<int>(data.confirmedBookings.size()); ++row) {
        const Booking& b = data.confirmedBookings[row];

        const QString dateIso = qs(b.getMarketDateId());
        const QString cat = categoryToQString(b.getCategory());

        auto* dateItem = new QTableWidgetItem(dateIso);
        auto* catItem  = new QTableWidgetItem(cat);

        // Store marketDateId for later actions
        dateItem->setData(Qt::UserRole, dateIso);

        m_bookingsTable->setItem(row, 0, dateItem);
        m_bookingsTable->setItem(row, 1, catItem);
    }

    // ---- Active Waitlists ----
    m_waitlistsTable->clearContents();
    m_waitlistsTable->setRowCount(static_cast<int>(data.activeWaitlists.size()));

    for (int row = 0; row < static_cast<int>(data.activeWaitlists.size()); ++row) {
        const WaitlistStatus& w = data.activeWaitlists[row];

        const QString dateIso = qs(w.dateIso);
        const QString cat = qs(w.category);
        const QString pos = QString::number(w.position);

        auto* dateItem = new QTableWidgetItem(dateIso);
        auto* catItem  = new QTableWidgetItem(cat);
        auto* posItem  = new QTableWidgetItem(pos);

        // Store marketDateId for later actions
        dateItem->setData(Qt::UserRole, qs(w.marketDateId));

        m_waitlistsTable->setItem(row, 0, dateItem);
        m_waitlistsTable->setItem(row, 1, catItem);
        m_waitlistsTable->setItem(row, 2, posItem);
    }

    // ---- Notifications ----
    m_notificationsBrowser->clear();
    if (data.notifications.empty()) {
        m_notificationsBrowser->setText(tr("No notifications."));
    } else {
        QStringList blocks;
        blocks.reserve(static_cast<int>(data.notifications.size()));
        for (auto it = data.notifications.rbegin(); it !=data.notifications.rend(); ++it) {
            blocks << qs(it->getMessage());
        }
        m_notificationsBrowser->setText(blocks.join("\n\n"));
    }

    // ---- Compliance Documents ----
    m_complianceTable->clearContents();
    m_complianceTable->setRowCount(static_cast<int>(data.complianceDocuments.size()));

    for (int row = 0; row < static_cast<int>(data.complianceDocuments.size()); ++row) {
        const ComplianceDocument& cd = data.complianceDocuments[row];

        const QString name = qs(cd.getDocumentName());
        const QString num  = qs(cd.getDocumentNumber());
        const QString exp  = qs(cd.getExpiryDate()); // already YYYY-MM-DD
        const QString valid = cd.isValid() ? "Yes" : "No";

        auto* nameItem = new QTableWidgetItem(name);
        // store legal text for view button
        nameItem->setData(Qt::UserRole, qs(cd.getLegalText()));

        m_complianceTable->setItem(row, 0, nameItem);
        m_complianceTable->setItem(row, 1, new QTableWidgetItem(num));
        m_complianceTable->setItem(row, 2, new QTableWidgetItem(exp));
        m_complianceTable->setItem(row, 3, new QTableWidgetItem(valid));
    }
}

QString VendorDashboardUI::selectedBookingMarketDateId() const {
    const int row = m_bookingsTable->currentRow();
    if (row < 0) return {};

    QTableWidgetItem* dateItem = m_bookingsTable->item(row, 0);
    if (!dateItem) return {};

    return dateItem->data(Qt::UserRole).toString();
}

QString VendorDashboardUI::selectedWaitlistMarketDateId() const {
    const int row = m_waitlistsTable->currentRow();
    if (row < 0) return {};

    QTableWidgetItem* dateItem = m_waitlistsTable->item(row, 0);
    if (!dateItem) return {};

    return dateItem->data(Qt::UserRole).toString();
}
