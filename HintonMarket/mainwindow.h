#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QStackedWidget;

class StartupUI;
class VendorDashboardUI;
class MarketScheduleUI;

// Backend
class InMemoryStorageManager;
class LoginController;
class DashboardController;
class WaitlistController;
class BookingController;
class MarketOperatorDashboardUI;
class MarketOperatorController;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void showStartup();
    void showDashboard();
    void showSchedule();
    void refreshDashboard();
    void refreshSchedule();
    void showOperatorDashboard();

    QStackedWidget* m_stack{};
    StartupUI* m_startup{};
    VendorDashboardUI* m_dashboard{};
    MarketScheduleUI* m_schedule{};

    std::string m_currentUserId;

    InMemoryStorageManager* m_storage{};
    LoginController* m_loginController{};
    DashboardController* m_dashboardController{};
    WaitlistController* m_waitlistController{};
    BookingController* m_bookingController{};
    MarketOperatorDashboardUI* m_operatorDashboard{};
    MarketOperatorController* m_operatorController{};

};

#endif // MAINWINDOW_H
