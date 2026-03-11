#include "BookingDialog.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

BookingDialog::BookingDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Confirm Booking"));
    setModal(true);

    m_titleLabel = new QLabel(tr("Confirm stall booking?"));
    QFont f = m_titleLabel->font();
    f.setBold(true);
    m_titleLabel->setFont(f);

    m_vendorLabel = new QLabel(tr("Vendor ID:"));
    m_dateLabel   = new QLabel(tr("Market Date:"));

    m_buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    m_buttons->button(QDialogButtonBox::Ok)->setText(tr("Confirm"));
    m_buttons->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));

    auto* root = new QVBoxLayout;
    root->addWidget(m_titleLabel);
    root->addSpacing(6);
    root->addWidget(m_vendorLabel);
    root->addWidget(m_dateLabel);
    root->addStretch(1);
    root->addWidget(m_buttons);
    setLayout(root);

    connect(m_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void BookingDialog::setVendorId(const QString& vendorId) {
    m_vendorLabel->setText(tr("Vendor ID: %1").arg(vendorId));
}

void BookingDialog::setMarketDateIso(const QString& dateIso) {
    m_dateLabel->setText(tr("Market Date: %1").arg(dateIso));
}
