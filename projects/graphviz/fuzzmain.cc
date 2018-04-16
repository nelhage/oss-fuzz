#include <graphviz/gvc.h>
#include <stdio.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    static GVC_t *gvc = NULL;
    if (gvc == NULL) {
        gvc = gvContext();
        agseterr(AGMAX);
    }
    FILE *fp = fmemopen((void*)data, size, "r");
    if (fp == NULL) {
        return 0;
    }
    Agraph_t* G = agread(fp, 0);
    fclose(fp);
    if (G != NULL) {
        agclose(G);
    }
    return 0;
}
