#ifndef COMPLIANCEDOCUMENTREPOSITORY_H
#define COMPLIANCEDOCUMENTREPOSITORY_H

#include <string>
#include <vector>

#include "ComplianceDocument.h"

class ComplianceDocumentRepository
{
public:
    std::vector<ComplianceDocument> loadDocumentsForVendor(const std::string& vendorId);
};

#endif // COMPLIANCEDOCUMENTREPOSITORY_H
