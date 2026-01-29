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
//<ID> 148
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = NULL;
    cmsCIExyY white;
    white.x = 0.3127;
    white.y = 0.3290;
    white.Y = 1.0;
    cmsHPROFILE hProfile = cmsCreateLab2Profile(&white);
    cmsUInt32Number headerFlags = cmsGetHeaderFlags(hProfile);
    cmsHANDLE it8 = cmsIT8Alloc(ctx);
    char patchName[64];
    int nPatch = (int)(headerFlags & 0xFFU);
    memset(patchName, 0, sizeof(patchName));
    cmsIT8GetPatchName(it8, nPatch, patchName);
    cmsPipeline * pipeline = cmsPipelineAlloc(ctx, 3, 3);
    cmsPipeline * pipelineDup = cmsPipelineDup(pipeline);
    cmsCloseProfile(hProfile);
    (void)ctx;
    (void)white;
    (void)headerFlags;
    (void)patchName;
    (void)it8;
    (void)nPatch;
    (void)pipeline;
    (void)pipelineDup;
    return 66;
}