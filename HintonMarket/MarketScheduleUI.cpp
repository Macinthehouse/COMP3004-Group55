#include "MarketScheduleUI.h"
#include "../DashboardController.h"

#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

static QTableWidgetItem* makeItem(const QString& text) {
    auto* it = new QTableWidgetItem(text);
    it->setFlags(it->flags() & ~Qt::ItemIsEditable);
    return it;
}

MarketScheduleUI::MarketScheduleUI(QWidget* parent) : QWidget(parent) {
    auto* title = new QLabel(tr("Market Schedule (Next 4 Weeks)"));
    QFont f = title->font();
    f.setPointSize(f.pointSize() + 3);
    f.setBold(true);
    title->setFont(f);

    m_backBtn = new QPushButton(tr("Back"));
    m_bookBtn = new QPushButton(tr("Book Stall"));
    m_waitlistBtn = new QPushButton(tr("Join Waitlist"));

    auto* topBar = new QHBoxLayout;
    topBar->addWidget(title);
    topBar->addStretch(1);
    topBar->addWidget(m_bookBtn);
    topBar->addWidget(m_waitlistBtn);
    topBar->addWidget(m_backBtn);

    m_datesTable = new QTableWidget(0, 3);
    m_datesTable->setHorizontalHeaderLabels({
        tr("Date"),
        tr("Food Available"),
        tr("Artisan Available")
    });
    m_datesTable->horizontalHeader()->setStretchLastSection(true);
    m_datesTable->verticalHeader()->setVisible(false);
    m_datesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_datesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_datesTable->setSelectionMode(QAbstractItemView::SingleSelection);

    auto* root = new QVBoxLayout;
    root->addLayout(topBar);
    root->addWidget(m_datesTable, 1);
    setLayout(root);

    connect(m_backBtn, &QPushButton::clicked, this, &MarketScheduleUI::backRequested);

    connect(m_bookBtn, &QPushButton::clicked, this, [this] {
        const QString marketDateId = selectedMarketDateId();
        if (marketDateId.isEmpty()) {
            QMessageBox::warning(this, tr("Book Stall"), tr("Please select a market date first."));
            return;
        }
        emit bookRequested(marketDateId);
    });

    connect(m_waitlistBtn, &QPushButton::clicked, this, [this] {
        const QString marketDateId = selectedMarketDateId();
        if (marketDateId.isEmpty()) {
            QMessageBox::warning(this, tr("Join Waitlist"), tr("Please select a market date first."));
            return;
        }
        emit joinWaitlistRequested(marketDateId);
    });
}

QString MarketScheduleUI::selectedMarketDateId() const {
    const int row = m_datesTable->currentRow();
    if (row < 0) return QString();

    QTableWidgetItem* dateItem = m_datesTable->item(row, 0);
    if (!dateItem) return QString();

    return dateItem->data(Qt::UserRole).toString();
}

void MarketScheduleUI::setSchedule(const std::vector<MarketDateSummary>& dates) {
    m_datesTable->clearContents();
    m_datesTable->setRowCount(static_cast<int>(dates.size()));

    for (int row = 0; row < static_cast<int>(dates.size()); ++row) {
        const MarketDateSummary& d = dates[row];

        const QString dateIso = QString::fromStdString(d.dateIso);
        const QString food = d.foodHasSpace ? "Yes" : "No";
        const QString art  = d.artisanHasSpace ? "Yes" : "No";

        auto* dateItem = makeItem(dateIso);
        dateItem->setData(Qt::UserRole, QString::fromStdString(d.marketDateId));

        m_datesTable->setItem(row, 0, dateItem);
        m_datesTable->setItem(row, 1, makeItem(food));
        m_datesTable->setItem(row, 2, makeItem(art));
    }

    m_datesTable->clearSelection();
}
