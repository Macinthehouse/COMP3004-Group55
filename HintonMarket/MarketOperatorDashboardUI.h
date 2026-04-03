#ifndef MARKETOPERATORDASHBOARDUI_H
#define MARKETOPERATORDASHBOARDUI_H

#include <QWidget>
#include <QString>

class QLabel;
class QPushButton;

class MarketOperatorDashboardUI : public QWidget {
    Q_OBJECT
public:
    explicit MarketOperatorDashboardUI(QWidget* parent = nullptr);

    void setOperatorId(const QString& operatorId);

signals:
    void bookForVendorRequested();
    void cancelForVendorRequested();
    void removeFromWaitlistRequested();
    void logoutRequested();

private:
    QLabel* m_headerLabel{};
    QLabel* m_operatorLabel{};

    QPushButton* m_bookBtn{};
    QPushButton* m_cancelBtn{};
    QPushButton* m_removeWaitlistBtn{};
    QPushButton* m_logoutBtn{};
};

#endif // MARKETOPERATORDASHBOARDUI_H
