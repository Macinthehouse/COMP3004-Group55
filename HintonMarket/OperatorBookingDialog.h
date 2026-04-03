#ifndef OPERATORBOOKINGDIALOG_H
#define OPERATORBOOKINGDIALOG_H

#include <QDialog>
#include <QString>

class QLineEdit;
class QDialogButtonBox;

class OperatorBookingDialog : public QDialog {
    Q_OBJECT
public:
    explicit OperatorBookingDialog(QWidget* parent = nullptr);

    QString vendorId() const;
    QString marketDateId() const;

    void setVendorId(const QString& v);
    void setMarketDateId(const QString& d);

private:
    QLineEdit* m_vendorEdit{};
    QLineEdit* m_dateEdit{};
    QDialogButtonBox* m_buttons{};
};

#endif // OPERATORBOOKINGDIALOG_H
