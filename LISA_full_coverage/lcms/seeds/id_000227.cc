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
//<ID> 227
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = (cmsContext)NULL;
    cmsUInt16Number alarms[16] = {0};
    alarms[0] = (cmsUInt16Number)0x0101U;
    alarms[1] = (cmsUInt16Number)0x0202U;
    cmsSetAlarmCodesTHR(ctx, alarms);

    // step 2: Create a profile and set/get header fields
    const cmsCIExyY * d50 = cmsD50_xyY();
    cmsHPROFILE hProfile = cmsCreateLab2ProfileTHR(ctx, d50);
    cmsSetHeaderManufacturer(hProfile, (cmsUInt32Number)0x1234U);
    cmsUInt32Number headerModel = cmsGetHeaderModel(hProfile);
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hProfile);

    // step 3: Work with MLU text objects
    cmsMLU * mlu = cmsMLUalloc(ctx, 1);
    cmsMLUsetASCII(mlu, "en", "US", "TestString");
    char buf[64];
    cmsUInt32Number got = cmsMLUgetASCII(mlu, "en", "US", buf, sizeof(buf));
    cmsMLU * mlu_dup = cmsMLUdup(mlu);

    // step 4: Tone curve operations
    cmsToneCurve * gamma = cmsBuildGamma(ctx, 2.2);
    cmsUInt16Number evaluated = cmsEvalToneCurve16(gamma, (cmsUInt16Number)12345U);
    cmsToneCurve * joined = cmsJoinToneCurve(ctx, gamma, gamma, (cmsUInt32Number)256U);
    cmsInt32Number paramType = cmsGetToneCurveParametricType(gamma);
    cmsToneCurve * reversed = cmsReverseToneCurveEx((cmsUInt32Number)128U, gamma);
    cmsToneCurve * duplicated = cmsDupToneCurve(gamma);
    cmsSmoothToneCurve(duplicated, 0.5);

    // step 5: Allocate a CLUT stage and inspect linkage
    const cmsFloat32Number clutTable[8] = { 0.0f, 0.25f, 0.5f, 0.75f, 1.0f, 0.9f, 0.1f, 0.6f };
    cmsStage * stage = cmsStageAllocCLutFloat(ctx, (cmsUInt32Number)2U, (cmsUInt32Number)3U, (cmsUInt32Number)3U, clutTable);
    cmsStage * next_stage = cmsStageNext(stage);

    // step 6: Try saving profile to memory to obtain required size
    cmsUInt32Number bytesNeeded = 0U;
    cmsBool saved_ok = cmsSaveProfileToMem(hProfile, (void *)NULL, &bytesNeeded);

    // Avoid unused variable warnings
    (void)headerModel;
    (void)manufacturer;
    (void)got;
    (void)mlu_dup;
    (void)evaluated;
    (void)joined;
    (void)paramType;
    (void)reversed;
    (void)duplicated;
    (void)next_stage;
    (void)saved_ok;
    (void)stage;
    (void)hProfile;
    (void)gamma;
    (void)mlu;
    (void)bytesNeeded;
    (void)alarms;

    // API sequence test completed successfully
    return 66;
}