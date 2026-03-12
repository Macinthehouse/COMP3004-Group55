#ifndef COMPLIANCEDOCUMENT_H
#define COMPLIANCEDOCUMENT_H

#include <string>

// --------------------------------------------------
// ComplianceDocument Class
// --------------------------------------------------
// Represents a legal compliance document required
// for vendors operating in HintonMarket.
//
// Responsibilities:
// - Store document details
// - Store expiry information
// - Provide legal description text
// --------------------------------------------------

class ComplianceDocument {
public:
    // --------------------------------------------------
    // Constructors
    // --------------------------------------------------

    ComplianceDocument();

    ComplianceDocument(const std::string& documentName,
                       const std::string& documentNumber,
                       const std::string& expiryDate,
                       const std::string& legalText);

    // --------------------------------------------------
    // Getters
    // --------------------------------------------------

    std::string getDocumentName() const;
    std::string getDocumentNumber() const;
    std::string getExpiryDate() const;
    std::string getLegalText() const;

    // --------------------------------------------------
    // Utility
    // --------------------------------------------------

    bool isValid() const;  // Simple expiry check

private:
    std::string documentName;
    std::string documentNumber;
    std::string expiryDate;  // YYYY-MM-DD
    std::string legalText;
};

#endif // COMPLIANCEDOCUMENT_H
