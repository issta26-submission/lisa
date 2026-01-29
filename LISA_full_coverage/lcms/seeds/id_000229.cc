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
//<ID> 229
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsUInt16Number alarms[16] = {0};
    alarms[0] = (cmsUInt16Number)0x0101U;
    alarms[1] = (cmsUInt16Number)0x0202U;
    alarms[2] = (cmsUInt16Number)0x0303U;

    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    cmsHPROFILE hProfile = cmsCreateXYZProfileTHR(ctx);

    cmsFloat64Number gamma_val = 2.2;
    cmsToneCurve * gamma = cmsBuildGamma(ctx, gamma_val);
    cmsToneCurve * curves[3];
    curves[0] = curves[1] = curves[2] = gamma;
    cmsStage * stage = cmsStageAllocToneCurves(ctx, (cmsUInt32Number)3, (cmsToneCurve *const*)curves);

    // step 2: Configure
    cmsSetAlarmCodesTHR(ctx, alarms);
    cmsSetDeviceClass(hProfile, (cmsProfileClassSignature)0x4D4F444CUL);

    // step 3: Operate
    const char * prop = cmsIT8GetPropertyMulti(hIT8, "Creator", NULL);
    alarms[3] = (cmsUInt16Number)((unsigned long)(prop) & 0xFFFFU);
    cmsStageSampleCLut16bit(stage, (cmsSAMPLER16)NULL, (void *)alarms, (cmsUInt32Number)0);

    // step 4: Validate & Cleanup
    cmsStageFree(stage);
    cmsCloseProfile(hProfile);

    // Keep references to avoid unused warnings in some build setups
    (void)hIT8;
    (void)ctx;
    (void)gamma;
    (void)curves;
    (void)prop;
    (void)alarms;

    return 66;
}