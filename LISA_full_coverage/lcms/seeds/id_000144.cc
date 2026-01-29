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
//<ID> 144
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsCIExyY whitePoint;
    cmsHPROFILE hLabProfile = NULL;
    cmsUInt32Number headerFlags = 0;
    cmsStage * stage = NULL;
    cmsContext stageCtx = NULL;
    cmsHANDLE it8 = NULL;
    char patchNameBuf[128];
    const char * obtainedName = NULL;
    double matrix[9];
    double offset[3];

    // step 2: Initialize data (no branches, straight-line)
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;

    matrix[0] = 1.0; matrix[1] = 0.0; matrix[2] = 0.0;
    matrix[3] = 0.0; matrix[4] = 1.0; matrix[5] = 0.0;
    matrix[6] = 0.0; matrix[7] = 0.0; matrix[8] = 1.0;
    offset[0] = 0.0; offset[1] = 0.0; offset[2] = 0.0;

    patchNameBuf[0] = '\0';

    // step 3: Core API sequence (straight-line)
    hLabProfile = cmsCreateLab2ProfileTHR(ctx, &whitePoint);
    headerFlags = cmsGetHeaderFlags(hLabProfile);

    stage = cmsStageAllocMatrix(ctx, 3, 3, (const cmsFloat64Number *)matrix, (const cmsFloat64Number *)offset);
    stageCtx = cmsGetStageContextID(stage);

    it8 = cmsIT8Alloc(ctx);
    obtainedName = cmsIT8GetPatchName(it8, 0, patchNameBuf);

    // step 4: Cleanup (straight-line)
    cmsIT8Free(it8);
    cmsCloseProfile(hLabProfile);

    (void)ctx;
    (void)headerFlags;
    (void)stage;
    (void)stageCtx;
    (void)obtainedName;
    (void)patchNameBuf;

    return 66;
}