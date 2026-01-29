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
//<ID> 140
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    const cmsCIExyY * d50 = cmsD50_xyY();
    cmsHPROFILE hProfile1 = NULL;
    cmsHPROFILE hProfile2 = NULL;
    cmsUInt32Number flags1 = 0;
    cmsUInt32Number flags2 = 0;
    cmsPipeline * pipeline = NULL;
    cmsContext pipelineCtx = NULL;
    cmsIOHANDLER * io = NULL;
    cmsHANDLE hIT8 = NULL;
    const char * prop = NULL;
    char memBuf[32];
    char scratch[128];

    // step 2: Setup
    memset(memBuf, 0, sizeof(memBuf));
    memset(scratch, 0, sizeof(scratch));
    hProfile1 = cmsCreateLab2ProfileTHR(ctx, d50);

    // step 3: Core operations
    flags1 = cmsGetHeaderFlags(hProfile1);
    pipeline = (cmsPipeline*) cmsReadTag(hProfile1, (cmsTagSignature) 0);
    pipelineCtx = cmsGetProfileContextID(hProfile1);
    hProfile2 = cmsCreateLab2ProfileTHR(pipelineCtx, d50);
    flags2 = cmsGetHeaderFlags(hProfile2);
    io = cmsOpenIOhandlerFromMem(pipelineCtx, memBuf, (cmsUInt32Number) sizeof(memBuf), "r");
    hIT8 = cmsIT8Alloc(pipelineCtx);
    prop = cmsIT8GetProperty(hIT8, "SOURCE");

    // step 4: Cleanup-like calls (only available APIs)
    cmsCloseIOhandler(io);

    (void)ctx;
    (void)d50;
    (void)hProfile1;
    (void)hProfile2;
    (void)flags1;
    (void)flags2;
    (void)pipeline;
    (void)pipelineCtx;
    (void)hIT8;
    (void)prop;
    (void)scratch;
    (void)memBuf;

    return 66;
}