#include "ComplianceDocument.h"

// --------------------------------------------------
// Default Constructor
// --------------------------------------------------

ComplianceDocument::ComplianceDocument()
{
}

// --------------------------------------------------
// Parameterized Constructor
// --------------------------------------------------

ComplianceDocument::ComplianceDocument(const std::string& documentName,
                                       const std::string& documentNumber,
                                       const std::string& expiryDate,
                                       const std::string& legalText)
    : documentName(documentName),
      documentNumber(documentNumber),
      expiryDate(expiryDate),
      legalText(legalText)
{
}

// --------------------------------------------------
// Getters
// --------------------------------------------------

std::string ComplianceDocument::getDocumentName() const
{
    return documentName;
}

std::string ComplianceDocument::getDocumentNumber() const
{
    return documentNumber;
}

std::string ComplianceDocument::getExpiryDate() const
{
    return expiryDate;
}

std::string ComplianceDocument::getLegalText() const
{
    return legalText;
}

// --------------------------------------------------
// isValid()
// --------------------------------------------------
// For Deliverable 1, we simplify validation.
// In real systems, we'd compare dates properly.
// Here we just check expiryDate is not empty.
// --------------------------------------------------

bool ComplianceDocument::isValid() const
{
    return !expiryDate.empty();
}