#include "MarketOperatorDashboardUI.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

MarketOperatorDashboardUI::MarketOperatorDashboardUI(QWidget* parent)
    : QWidget(parent)
{
    m_headerLabel = new QLabel(tr("Market Operator Dashboard"));
    QFont f = m_headerLabel->font();
    f.setPointSize(f.pointSize() + 3);
    f.setBold(true);
    m_headerLabel->setFont(f);

    m_operatorLabel = new QLabel(tr("Operator ID: (not loaded)"));

    m_bookBtn = new QPushButton(tr("Book Stall for Vendor..."));
    m_cancelBtn = new QPushButton(tr("Cancel Booking for Vendor..."));
    m_removeWaitlistBtn = new QPushButton(tr("Remove Vendor from Waitlist..."));
    m_logoutBtn = new QPushButton(tr("Logout"));

    auto* buttons = new QVBoxLayout;
    buttons->addWidget(m_bookBtn);
    buttons->addWidget(m_cancelBtn);
    buttons->addWidget(m_removeWaitlistBtn);
    buttons->addStretch(1);

    auto* topBar = new QHBoxLayout;
    topBar->addWidget(m_headerLabel);
    topBar->addStretch(1);
    topBar->addWidget(m_logoutBtn);

    auto* root = new QVBoxLayout;
    root->addLayout(topBar);
    root->addWidget(m_operatorLabel);
    root->addSpacing(10);
    root->addLayout(buttons);
    setLayout(root);

    connect(m_bookBtn, &QPushButton::clicked, this, &MarketOperatorDashboardUI::bookForVendorRequested);
    connect(m_cancelBtn, &QPushButton::clicked, this, &MarketOperatorDashboardUI::cancelForVendorRequested);
    connect(m_removeWaitlistBtn, &QPushButton::clicked, this, &MarketOperatorDashboardUI::removeFromWaitlistRequested);
    connect(m_logoutBtn, &QPushButton::clicked, this, &MarketOperatorDashboardUI::logoutRequested);
}

void MarketOperatorDashboardUI::setOperatorId(const QString& operatorId)
{
    m_operatorLabel->setText(tr("Operator ID: %1").arg(operatorId));
}
