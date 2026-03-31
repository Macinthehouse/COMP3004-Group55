#ifndef STARTUPUI_H
#define STARTUPUI_H

#include <QWidget>

class QLineEdit;
class QPushButton;

class StartupUI : public QWidget {
    Q_OBJECT
public:
    explicit StartupUI(QWidget* parent = nullptr);

    QString userId() const;
void setUserId(const QString& text);
QLineEdit* m_userIdEdit{};
signals:
    void loginRequested(const QString& userId);

private:
    QLineEdit* m_usernameEdit{};
    QPushButton* m_loginBtn{};
};

#endif
