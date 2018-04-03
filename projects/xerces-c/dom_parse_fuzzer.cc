#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/dom/DOMError.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXParseException.hpp>

XERCES_CPP_NAMESPACE_USE

class DOMTreeErrorReporter : public ErrorHandler {
public:
    void warning(const SAXParseException& toCatch) {}
    void error(const SAXParseException& toCatch) {}
    void fatalError(const SAXParseException& toCatch) {}
    void resetErrors() {}
};


extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    try {
         XMLPlatformUtils::Initialize();
    } catch (...) {
         return 1;
    }

    XercesDOMParser *parser = new XercesDOMParser;
    // We can set this to Val_Never, Val_Auto, or Val_Always
    parser->setValidationScheme(XercesDOMParser::Val_Never);
    parser->setDoNamespaces(false);
    parser->setDoSchema(false);
    parser->setHandleMultipleImports(true);
    parser->setValidationSchemaFullChecking(false);
    parser->setCreateEntityReferenceNodes(false);
    DOMTreeErrorReporter *reporter = new DOMTreeErrorReporter;
    parser->setErrorHandler(reporter);
    MemBufInputSource* mem_buf = new MemBufInputSource((const XMLByte*)data, size, "fuzzData", false);

    try {
        parser->parse(*mem_buf);
    } catch (...) {
    }
    delete mem_buf;
    delete parser;
    delete reporter;
    XMLPlatformUtils::Terminate();
    return 0;
}
