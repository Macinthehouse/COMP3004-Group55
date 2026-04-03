#ifndef STARTUPUI_H
#define STARTUPUI_H

#include <QWidget>
#include <QString>

class QLineEdit;
class QPushButton;

class StartupUI : public QWidget {
    Q_OBJECT
public:
    explicit StartupUI(QWidget* parent = nullptr);

    QString username() const;
    void setUsername(const QString& text);
    void clear();

signals:
    void loginRequested(const QString& username);

private:
    QLineEdit* m_usernameEdit{};
    QPushButton* m_loginBtn{};
};

#endif // STARTUPUI_H
