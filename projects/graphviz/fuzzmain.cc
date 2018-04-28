#include <graphviz/gvc.h>
#include <stdio.h>
#include <stdlib.h>

// Set our own error-handling function to discard all errors. If we
// instead set the error level using `agseterr`, all errors get
// redirected to a tmpfile we have no way of cleaning up.
int null_errf(char *msg) {
    return 0;
}

const char *kLayoutEngine = "dot";
const char *kFormat = "svg";

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    static bool initialized = false;
    if (!initialized) {
        agseterrf(null_errf);
    }
    FILE *fp = fmemopen((void*)data, size, "r");
    if (fp == NULL) {
        return 0;
    }
    Agraph_t* G = agread(fp, 0);
    fclose(fp);
    if (G == NULL) {
        return 0;
    }

    // serialize back to text
    char *buffer = nullptr;
    size_t buflen;

    FILE *out = open_memstream(&buffer, &buflen);
    agwrite(G, out);
    fclose(out);
    free(buffer);

    // layout + render
    auto *gvc = gvContext();
    gvLayout(gvc, G, kLayoutEngine);
    gvFreeLayout(gvc, G);

    out = open_memstream(&buffer, &buflen);
    gvRender(gvc, G, kFormat, out);
    fclose(out);
    free(buffer);

    agclose(G);
    gvFreeContext(gvc);
    return 0;
}
