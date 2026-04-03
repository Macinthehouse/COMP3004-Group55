#ifndef OPERATORCANCELLATIONDIALOG_H
#define OPERATORCANCELLATIONDIALOG_H

#include <QDialog>
#include <QString>

class QLineEdit;
class QDialogButtonBox;

class OperatorCancellationDialog : public QDialog {
    Q_OBJECT
public:
    explicit OperatorCancellationDialog(QWidget* parent = nullptr);

    QString vendorId() const;
    QString marketDateId() const;

    void setVendorId(const QString& v);
    void setMarketDateId(const QString& d);

private:
    QLineEdit* m_vendorEdit{};
    QLineEdit* m_dateEdit{};
    QDialogButtonBox* m_buttons{};
};

#endif // OPERATORCANCELLATIONDIALOG_H
