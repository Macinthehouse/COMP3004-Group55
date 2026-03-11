#ifndef MARKETSCHEDULEUI_H
#define MARKETSCHEDULEUI_H

#include <QWidget>
#include <QString>
#include <vector>

struct MarketDateSummary;

class QPushButton;
class QTableWidget;

class MarketScheduleUI : public QWidget {
    Q_OBJECT
public:
    explicit MarketScheduleUI(QWidget* parent = nullptr);

    void setSchedule(const std::vector<MarketDateSummary>& dates);

signals:
    void backRequested();
    void bookRequested(const QString& marketDateId);
    void joinWaitlistRequested(const QString& marketDateId);

private:
    QString selectedMarketDateId() const;

    QTableWidget* m_datesTable{};
    QPushButton* m_bookBtn{};
    QPushButton* m_waitlistBtn{};
    QPushButton* m_backBtn{};
};

#endif // MARKETSCHEDULEUI_H
