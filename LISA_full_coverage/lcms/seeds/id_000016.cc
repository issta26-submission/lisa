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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE prof_bchs = NULL;
    cmsHPROFILE prof_ink = NULL;
    cmsUInt32Number headerFlags = 0;
    cmsStage * clut_stage = NULL;
    cmsFloat32Number * table = NULL;
    cmsUInt32Number nGridPoints = 3U;
    cmsUInt32Number inputChan = 3U;
    cmsUInt32Number outputChan = 3U;
    size_t tableSize = (size_t)nGridPoints * (size_t)nGridPoints * (size_t)nGridPoints * (size_t)outputChan;
    char patchBuf[64];
    cmsHANDLE it8 = NULL;
    const char * patchName = NULL;
    cmsColorSpaceSignature cs = _cmsICCcolorSpace(0);

    // step 2: Setup - allocate table and create profiles and IT8 handle
    table = (cmsFloat32Number *) malloc(tableSize * sizeof(cmsFloat32Number));
    memset(table, 0, tableSize * sizeof(cmsFloat32Number));
    prof_bchs = cmsCreateBCHSWabstractProfileTHR(ctx, 17U, 1.0, 1.0, 0.0, 1.0, 5000U, 6500U);
    prof_ink = cmsCreateInkLimitingDeviceLinkTHR(ctx, cs, 0.5);
    it8 = cmsIT8Alloc(ctx);

    // step 3: Operate & Validate - allocate CLUT stage, query header flags and IT8 patch name
    clut_stage = cmsStageAllocCLutFloat(ctx, nGridPoints, inputChan, outputChan, table);
    headerFlags = cmsGetHeaderFlags(prof_bchs);
    (void) cmsStageInputChannels(clut_stage);
    patchName = cmsIT8GetPatchName(it8, 0, patchBuf);
    (void) patchName;
    (void) headerFlags;

    // step 4: Cleanup
    cmsStageFree(clut_stage);
    cmsIT8Free(it8);
    free(table);
    prof_bchs = NULL;
    prof_ink = NULL;

    // API sequence test completed successfully
    return 66;
}