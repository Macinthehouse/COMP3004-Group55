#include "WaitlistDialog.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

WaitlistDialog::WaitlistDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Waitlist"));
    setModal(true);

    m_titleLabel = new QLabel;
    QFont f = m_titleLabel->font();
    f.setBold(true);
    m_titleLabel->setFont(f);

    m_vendorLabel = new QLabel;
    m_dateLabel   = new QLabel;

    m_buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

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

    updateText();
}

void WaitlistDialog::setMode(Mode mode) {
    m_mode = mode;
    updateText();
}

void WaitlistDialog::setVendorId(const QString& vendorId) {
    m_vendorId = vendorId;
    updateText();
}

void WaitlistDialog::setMarketDateIso(const QString& dateIso) {
    m_dateIso = dateIso;
    updateText();
}

void WaitlistDialog::updateText() {
    const bool joining = (m_mode == Mode::Join);

    setWindowTitle(joining ? tr("Confirm Join Waitlist") : tr("Confirm Leave Waitlist"));
    m_titleLabel->setText(joining ? tr("Join the waitlist?") : tr("Leave the waitlist?"));

    m_vendorLabel->setText(tr("Vendor ID: %1").arg(m_vendorId.isEmpty() ? tr("(unknown)") : m_vendorId));
    m_dateLabel->setText(tr("Market Date: %1").arg(m_dateIso.isEmpty() ? tr("(unknown)") : m_dateIso));

    if (auto* ok = m_buttons->button(QDialogButtonBox::Ok)) {
        ok->setText(joining ? tr("Join") : tr("Leave"));
    }
    if (auto* cancel = m_buttons->button(QDialogButtonBox::Cancel)) {
        cancel->setText(tr("Cancel"));
    }
}
