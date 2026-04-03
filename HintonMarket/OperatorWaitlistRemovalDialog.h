#ifndef OPERATORWAITLISTREMOVALDIALOG_H
#define OPERATORWAITLISTREMOVALDIALOG_H

#include <QDialog>
#include <QString>

class QLineEdit;
class QDialogButtonBox;

class OperatorWaitlistRemovalDialog : public QDialog {
    Q_OBJECT
public:
    explicit OperatorWaitlistRemovalDialog(QWidget* parent = nullptr);

    QString vendorId() const;
    QString marketDateId() const;

    void setVendorId(const QString& v);
    void setMarketDateId(const QString& d);

private:
    QLineEdit* m_vendorEdit{};
    QLineEdit* m_dateEdit{};
    QDialogButtonBox* m_buttons{};
};

#endif // OPERATORWAITLISTREMOVALDIALOG_H
