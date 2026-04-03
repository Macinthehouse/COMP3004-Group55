

#include "StartupUI.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

StartupUI::StartupUI(QWidget* parent) : QWidget(parent) {
    auto* title = new QLabel(tr("HintonMarket"));
    QFont f = title->font();
    f.setPointSize(f.pointSize() + 4);
    f.setBold(true);
    title->setFont(f);

    auto* prompt = new QLabel(tr("Enter Vendor ID / Username:"));

    m_usernameEdit = new QLineEdit;
    m_usernameEdit->setPlaceholderText(tr("e.g., food_vendor_1"));

    m_loginBtn = new QPushButton(tr("Login"));

    auto* formRow = new QHBoxLayout;
    formRow->addWidget(prompt);
    formRow->addWidget(m_usernameEdit, 1);
    formRow->addWidget(m_loginBtn);

    auto* root = new QVBoxLayout;
    root->addWidget(title);
    root->addSpacing(8);
    root->addLayout(formRow);
    root->addStretch(1);
    setLayout(root);

    connect(m_loginBtn, &QPushButton::clicked, this, [this] {
        emit loginRequested(m_usernameEdit->text().trimmed());
    });
    connect(m_usernameEdit, &QLineEdit::returnPressed, m_loginBtn, &QPushButton::click);
}

QString StartupUI::username() const { return m_usernameEdit->text(); }
void StartupUI::setUsername(const QString& text) { m_usernameEdit->setText(text); }
void StartupUI::clear() { m_usernameEdit->clear(); }
