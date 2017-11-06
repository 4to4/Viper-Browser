#include "HistoryWidget.h"
#include "ui_historywidget.h"
#include "HistoryManager.h"
#include "HistoryTableModel.h"

#include <algorithm>
#include <QDateTime>
#include <QList>
#include <QMenu>
#include <QRegExp>
#include <QResizeEvent>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>

HistoryWidget::HistoryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryWidget),
    m_proxyModel(new QSortFilterProxyModel(this)),
    m_timeRange(HistoryRange::Week)
{
    ui->setupUi(this);

    // Set properties of proxy model used for bookmark searches
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setFilterKeyColumn(-1); // -1 applies search terms to all columns

    // Enable search for history
    connect(ui->lineEditSearch, &QLineEdit::editingFinished, this, &HistoryWidget::searchHistory);

    // Set up criteria list
    setupCriteriaList();
}

HistoryWidget::~HistoryWidget()
{
    delete ui;
}

void HistoryWidget::setHistoryManager(HistoryManager *manager)
{
    HistoryTableModel *tableModel = new HistoryTableModel(manager, this);
    m_proxyModel->setSourceModel(tableModel);
    ui->tableView->setModel(m_proxyModel);

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &HistoryWidget::onContextMenuRequested);
}

void HistoryWidget::loadHistory()
{
    static_cast<HistoryTableModel*>(m_proxyModel->sourceModel())->loadFromDate(getLoadDate());
}

void HistoryWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    int tableWidth = ui->tableView->geometry().width();
    ui->tableView->setColumnWidth(0, std::max(tableWidth / 4, 0));
    ui->tableView->setColumnWidth(1, std::max(tableWidth / 2 - 5, 0));
    ui->tableView->setColumnWidth(2, std::max(tableWidth / 4, 0));
}

void HistoryWidget::onContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->tableView->indexAt(pos);
    if (!index.isValid())
        return;

    QMenu menu(this);
    menu.addAction(tr("Open"), [=](){
        emit openLink(static_cast<HistoryTableModel*>(m_proxyModel->sourceModel())->getIndexURL(index));
    });
    menu.addAction(tr("Open in a new tab"), [=](){
        emit openLinkNewTab(static_cast<HistoryTableModel*>(m_proxyModel->sourceModel())->getIndexURL(index));
    });
    menu.addAction(tr("Open in a new window"), [=](){
        emit openLinkNewWindow(static_cast<HistoryTableModel*>(m_proxyModel->sourceModel())->getIndexURL(index));
    });
    menu.exec(ui->tableView->mapToGlobal(pos));
}

void HistoryWidget::onCriteriaChanged(const QModelIndex &index)
{
    int indexData = index.data(Qt::UserRole).toInt();
    if (indexData >= static_cast<int>(HistoryRange::RangeMax) || indexData < 0)
        return;

    HistoryRange newRange = static_cast<HistoryRange>(indexData);
    if (m_timeRange != newRange)
    {
        m_timeRange = newRange;
        loadHistory();
    }
}

void HistoryWidget::searchHistory()
{
    m_proxyModel->setFilterRegExp(ui->lineEditSearch->text());
}

void HistoryWidget::setupCriteriaList()
{
    QStringList listItems;
    listItems << "Last 7 days"
              << "Last 14 days"
              << "Last month"
              << "Last year"
              << "All";
    ui->listWidgetCriteria->addItems(listItems);

    // Associate user data type (HistoryTypes) with widget items
    QListWidgetItem *item = ui->listWidgetCriteria->item(0);
    item->setData(Qt::UserRole, static_cast<int>(HistoryRange::Week));
    item = ui->listWidgetCriteria->item(1);
    item->setData(Qt::UserRole, static_cast<int>(HistoryRange::Fortnight));
    item = ui->listWidgetCriteria->item(2);
    item->setData(Qt::UserRole, static_cast<int>(HistoryRange::Month));
    item = ui->listWidgetCriteria->item(3);
    item->setData(Qt::UserRole, static_cast<int>(HistoryRange::Year));
    item = ui->listWidgetCriteria->item(4);
    item->setData(Qt::UserRole, static_cast<int>(HistoryRange::All));

    ui->listWidgetCriteria->setCurrentRow(0);

    connect(ui->listWidgetCriteria, &QListWidget::clicked, this, &HistoryWidget::onCriteriaChanged);
}

QDateTime HistoryWidget::getLoadDate()
{
    QDateTime retDate = QDateTime::currentDateTime();
    qint64 timeSubtract = 0;
    switch (m_timeRange)
    {
        case HistoryRange::Week: timeSubtract = -7; break;
        case HistoryRange::Fortnight: timeSubtract = -14; break;
        case HistoryRange::Month: timeSubtract = -31; break;
        case HistoryRange::Year: timeSubtract = -365; break;
        case HistoryRange::All: timeSubtract = -6000; break;
        default: break;
    }
    return retDate.addDays(timeSubtract);
}
