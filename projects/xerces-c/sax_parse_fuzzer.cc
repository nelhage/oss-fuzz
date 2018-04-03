#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/sax/HandlerBase.hpp>

XERCES_CPP_NAMESPACE_USE

XERCES_CPP_NAMESPACE_BEGIN
class AttributeList;
XERCES_CPP_NAMESPACE_END


class MemParseHandlers : public HandlerBase {
public:
    void startElement(const XMLCh* const name, AttributeList& attributes) {}
    void characters(const XMLCh* const chars, const XMLSize_t length) {}
    void ignorableWhitespace(const XMLCh* const chars, const XMLSize_t length) {}
    void resetDocument() {}
};


extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    try {
         XMLPlatformUtils::Initialize();
    } catch (...) {
         return 1;
    }

    MemParseHandlers handler;
    SAXParser *parser = new SAXParser;
    // We can set this to Val_Never, Val_Auto, or Val_Always
    parser->setValidationScheme(SAXParser::Val_Never);
    parser->setDoNamespaces(false);
    parser->setDoSchema(false);
    parser->setHandleMultipleImports(true);
    parser->setValidationSchemaFullChecking(false);
    parser->setDocumentHandler(&handler);
    parser->setErrorHandler(&handler);
    MemBufInputSource* mem_buf = new MemBufInputSource((const XMLByte*)data, size, "fuzzData", false);

    try {
        parser->parse(*mem_buf);
    } catch (...) {
    }
    delete mem_buf;
    delete parser;
    XMLPlatformUtils::Terminate();
    return 0;
}
