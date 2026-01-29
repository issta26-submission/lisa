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
//<ID> 346
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = (cmsContext) NULL;
    cmsHPROFILE profile = cmsCreateNULLProfileTHR(ctx);
    cmsHPROFILE profiles[1];
    profiles[0] = profile;
    cmsHTRANSFORM xform = cmsCreateMultiprofileTransform(profiles, (cmsUInt32Number)1, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsNAMEDCOLORLIST * namedList = cmsGetNamedColorList(xform);
    cmsFloat64Number clutTable[24];
    clutTable[0]  = 0.0;  clutTable[1]  = 0.25; clutTable[2]  = 0.5;
    clutTable[3]  = 0.1;  clutTable[4]  = 0.35; clutTable[5]  = 0.6;
    clutTable[6]  = 0.2;  clutTable[7]  = 0.45; clutTable[8]  = 0.7;
    clutTable[9]  = 0.3;  clutTable[10] = 0.55; clutTable[11] = 0.8;
    clutTable[12] = 0.4;  clutTable[13] = 0.65; clutTable[14] = 0.9;
    clutTable[15] = 0.5;  clutTable[16] = 0.75; clutTable[17] = 1.0;
    clutTable[18] = 0.6;  clutTable[19] = 0.85; clutTable[20] = 0.15;
    clutTable[21] = 0.7;  clutTable[22] = 0.95; clutTable[23] = 0.05;
    cmsFloat64Number matrix[9];
    matrix[0] = 1.0; matrix[1] = 0.0; matrix[2] = 0.0;
    matrix[3] = 0.0; matrix[4] = 1.0; matrix[5] = 0.0;
    matrix[6] = 0.0; matrix[7] = 0.0; matrix[8] = 1.0;
    cmsStage * clutStage = cmsStageAllocMatrix(ctx, (cmsUInt32Number)3, (cmsUInt32Number)3, matrix, (const cmsFloat64Number *) NULL);
    cmsFloat64Number sample = cmsDetectTAC(profile);
    clutTable[0] = clutTable[0] + sample;
    (void) namedList;
    (void) clutStage;
    (void) xform;
    cmsUnregisterPluginsTHR(ctx);
    return 66;
}