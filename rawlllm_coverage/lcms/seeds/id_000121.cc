#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 121
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Use a NULL context (default) for THR functions
    cmsContext ctx = NULL;

    // 1) Create an sRGB profile using the threaded variant
    cmsHPROFILE profile = cmsCreate_sRGBProfileTHR(ctx);
    if (profile == NULL) {
        return 1;
    }

    // 2) Allocate a simple 3->3 pipeline
    cmsPipeline * pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (pipeline == NULL) {
        // Can't free profile via listed APIs, but bail out with error
        return 2;
    }

    // 3) Allocate a named color list, append one color, then free it
    cmsNAMEDCOLORLIST * ncl = cmsAllocNamedColorList(ctx, 1, 4, "Pre", "Suf");
    if (ncl != NULL) {
        // Prepare sample PCS and colorant arrays
        cmsUInt16Number PCS[3] = { 0, 0, 0 };
        cmsUInt16Number Colorant[16] = { 0 };
        // Append a named color (if available)
        cmsAppendNamedColor(ncl, "ExampleColor", PCS, Colorant);
        // Free the named color list (required API)
        cmsFreeNamedColorList(ncl);
    }

    // 4) Demonstrate cmsMLUgetASCII usage with a small buffer.
    // Use a NULL cmsMLU pointer here (safe for compile-time demonstration).
    char buffer[128] = { 0 };
    const char lang[3] = { 'e', 'n', 0 };
    const char country[3] = { 'U', 'S', 0 };
    cmsUInt32Number len = cmsMLUgetASCII(NULL, lang, country, buffer, sizeof(buffer));
    (void)len; // suppress unused-variable warning; buffer may be empty when mlu==NULL

    // Note: There are no listed APIs to free 'pipeline' or 'profile' in the provided set,
    // so we leave them as-is for this synthesis test.

    return 66;
}