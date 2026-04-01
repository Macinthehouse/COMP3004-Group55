#include "ComplianceDocumentRepository.h"
#include "DatabaseManager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

std::vector<ComplianceDocument> ComplianceDocumentRepository::loadDocumentsForVendor(const std::string& vendorId)
{
    std::vector<ComplianceDocument> documents;

    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT document_type, document_number, expiry_date, legal_text "
        "FROM compliance_documents "
        "WHERE vendor_id = :vendor_id "
        "ORDER BY id"
    );
    query.bindValue(":vendor_id", QString::fromStdString(vendorId));

    if (!query.exec()) {
        qDebug() << "ComplianceDocumentRepository::loadDocumentsForVendor failed:"
                 << query.lastError().text();
        return documents;
    }

    while (query.next()) {
        documents.emplace_back(
            query.value("document_type").toString().toStdString(),
            query.value("document_number").toString().toStdString(),
            query.value("expiry_date").toString().toStdString(),
            query.value("legal_text").toString().toStdString()
        );
    }

    return documents;
}
