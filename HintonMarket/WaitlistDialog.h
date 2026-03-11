#ifndef WAITLISTDIALOG_H
#define WAITLISTDIALOG_H

#include <QDialog>
#include <QString>

class QLabel;
class QDialogButtonBox;

class WaitlistDialog : public QDialog {
    Q_OBJECT
public:
    enum class Mode {
        Join,
        Leave
    };

    explicit WaitlistDialog(QWidget* parent = nullptr);

    void setMode(Mode mode);
    void setVendorId(const QString& vendorId);
    void setMarketDateIso(const QString& dateIso);

private:
    void updateText();

    Mode m_mode{Mode::Join};
    QString m_vendorId;
    QString m_dateIso;

    QLabel* m_titleLabel{};
    QLabel* m_vendorLabel{};
    QLabel* m_dateLabel{};
    QDialogButtonBox* m_buttons{};
};

#endif // WAITLISTDIALOG_H
