#include "OperatorWaitlistRemovalDialog.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

OperatorWaitlistRemovalDialog::OperatorWaitlistRemovalDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Remove from Waitlist (Operator)"));
    setModal(true);

    auto* intro = new QLabel(tr("Enter the vendor ID and market date to remove the vendor from that waitlist."));
    intro->setWordWrap(true);

    m_vendorEdit = new QLineEdit;
    m_vendorEdit->setPlaceholderText(tr("e.g., food_vendor_1"));

    m_dateEdit = new QLineEdit;
    m_dateEdit->setPlaceholderText(tr("YYYY-MM-DD (e.g., 2026-04-08)"));

    auto* form = new QFormLayout;
    form->addRow(tr("Vendor ID:"), m_vendorEdit);
    form->addRow(tr("Market Date:"), m_dateEdit);

    m_buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    m_buttons->button(QDialogButtonBox::Ok)->setText(tr("Remove"));
    m_buttons->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));

    auto* root = new QVBoxLayout;
    root->addWidget(intro);
    root->addLayout(form);
    root->addWidget(m_buttons);
    setLayout(root);

    connect(m_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QString OperatorWaitlistRemovalDialog::vendorId() const { return m_vendorEdit->text().trimmed(); }
QString OperatorWaitlistRemovalDialog::marketDateId() const { return m_dateEdit->text().trimmed(); }

void OperatorWaitlistRemovalDialog::setVendorId(const QString& v) { m_vendorEdit->setText(v); }
void OperatorWaitlistRemovalDialog::setMarketDateId(const QString& d) { m_dateEdit->setText(d); }
