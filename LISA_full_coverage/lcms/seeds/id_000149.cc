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
//<ID> 149
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext initialCtx = cmsCreateContext(NULL, NULL);
    cmsContext pipeCtx = NULL;
    cmsContext profCtx = NULL;
    cmsCIExyY whitePoint;
    cmsHPROFILE hProf1 = NULL;
    cmsHPROFILE hProf2 = NULL;
    cmsHANDLE hIT8 = NULL;
    const char * patchName = NULL;
    cmsUInt32Number headerFlags = 0;
    cmsPipeline * lut = NULL;

    // step 2: Setup white point
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;

    // step 3: Core operations (straight-line, no branches)
    lut = cmsPipelineAlloc(initialCtx, 3, 3);
    pipeCtx = cmsGetPipelineContextID(lut);
    hProf1 = cmsCreateLab2Profile(&whitePoint);
    headerFlags = cmsGetHeaderCreator(hProf1);
    hProf2 = cmsCreateXYZProfileTHR(pipeCtx);
    profCtx = cmsGetProfileContextID(hProf2);
    patchName = NULL; /* original code attempted to query IT8; avoid calling IT8 APIs with a NULL handle */

    // step 4: Cleanup
    cmsPipelineFree(lut);

    (void)initialCtx;
    (void)pipeCtx;
    (void)profCtx;
    (void)whitePoint;
    (void)hProf1;
    (void)hProf2;
    (void)hIT8;
    (void)patchName;
    (void)headerFlags;

    // API sequence test completed successfully
    return 66;
}