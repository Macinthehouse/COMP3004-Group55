#ifndef BOOKINGDIALOG_H
#define BOOKINGDIALOG_H

#include <QDialog>
#include <QString>

class QLabel;
class QDialogButtonBox;

class BookingDialog : public QDialog {
    Q_OBJECT
public:
    explicit BookingDialog(QWidget* parent = nullptr);

    void setVendorId(const QString& vendorId);
    void setMarketDateIso(const QString& dateIso);

private:
    QLabel* m_titleLabel{};
    QLabel* m_vendorLabel{};
    QLabel* m_dateLabel{};
    QDialogButtonBox* m_buttons{};
};

#endif // BOOKINGDIALOG_H
