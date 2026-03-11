#ifndef VENDORDASHBOARDUI_H
#define VENDORDASHBOARDUI_H

#include <QWidget>
#include <QString>

class QLabel;
class QPushButton;
class QTableWidget;
class QTextBrowser;

class ComplianceDocument;

struct VendorDashboardData;

class VendorDashboardUI : public QWidget {
    Q_OBJECT
public:
    explicit VendorDashboardUI(QWidget* parent = nullptr);

    void setVendorTitle(const QString& vendorId);
    void setDashboardData(const VendorDashboardData& data);

signals:
    void browseMarketDatesRequested();
    void refreshRequested();
    void logoutRequested();

    void cancelBookingRequested(const QString& marketDateId);
    void leaveWaitlistRequested(const QString& marketDateId);

    void viewComplianceTextRequested(const QString& documentName, const QString& legalText);

private:
    QString selectedBookingMarketDateId() const;
    QString selectedWaitlistMarketDateId() const;

    QLabel* m_vendorSummaryLabel{};
    QTableWidget* m_bookingsTable{};
    QTableWidget* m_waitlistsTable{};
    QTextBrowser* m_notificationsBrowser{};

    QPushButton* m_browseBtn{};
    QPushButton* m_refreshBtn{};
    QPushButton* m_logoutBtn{};

    QPushButton* m_cancelBookingBtn{};
    QPushButton* m_leaveWaitlistBtn{};

    QTableWidget* m_complianceTable{};
    QPushButton* m_viewComplianceTextBtn{};
};

#endif
