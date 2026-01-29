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
//<ID> 146
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsCIExyY whitePoint;
    cmsHPROFILE hProfile = NULL;
    cmsUInt32Number headerFlags = 0;
    cmsPipeline * pipeline = NULL;
    cmsContext pipelineCtx = NULL;
    cmsUInt32Number headerCreator = 0;
    char patchBuf[128];
    const char * dummy = NULL;

    // step 2: Setup
    memset(patchBuf, 0, sizeof(patchBuf));
    cmsWhitePointFromTemp(&whitePoint, 5000.0);
    hProfile = cmsCreateLab4ProfileTHR(ctx, &whitePoint);

    // step 3: Core operations (straight-line, no branches)
    headerFlags = cmsGetHeaderFlags(hProfile);
    pipeline = cmsPipelineAlloc(ctx, 3, 3);
    pipelineCtx = cmsGetPipelineContextID(pipeline);
    headerCreator = cmsGetHeaderCreator(hProfile);

    // step 4: Cleanup
    cmsCloseProfile(hProfile);
    (void)ctx;
    (void)whitePoint;
    (void)headerFlags;
    (void)pipeline;
    (void)pipelineCtx;
    (void)headerCreator;
    (void)patchBuf;
    (void)dummy;

    // API sequence test completed successfully
    return 66;
}