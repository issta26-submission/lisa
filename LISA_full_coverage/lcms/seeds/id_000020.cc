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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & initialization
    cmsContext ctx = (cmsContext) NULL;
    cmsIOHANDLER * ioh = (cmsIOHANDLER *) NULL;
    unsigned char profileMem[32];
    memset(profileMem, 0, sizeof(profileMem));
    cmsHPROFILE profile = (cmsHPROFILE) NULL;
    cmsMLU * mlu = (cmsMLU *) NULL;
    wchar_t wideBuffer[64];
    cmsUInt32Number wideCount = 0;
    cmsUInt16Number tabValues[2];
    tabValues[0] = 0U;
    tabValues[1] = 65535U;
    cmsToneCurve * curve1 = (cmsToneCurve *) NULL;
    cmsToneCurve * curve2 = (cmsToneCurve *) NULL;
    cmsToneCurve * curve3 = (cmsToneCurve *) NULL;
    cmsToneCurve * curves[3] = { NULL, NULL, NULL };
    cmsStage * stage = (cmsStage *) NULL;
    cmsUInt32Number inputChannels = 0;

    // step 2: Setup (IO handler, profile in memory, build tone curves)
    ioh = cmsOpenIOhandlerFromNULL(ctx);
    profile = cmsOpenProfileFromMem((const void *) profileMem, (cmsUInt32Number) sizeof(profileMem));
    curve1 = cmsBuildTabulatedToneCurve16(ctx, 2U, tabValues);
    curve2 = cmsBuildTabulatedToneCurve16(ctx, 2U, tabValues);
    curve3 = cmsBuildTabulatedToneCurve16(ctx, 2U, tabValues);
    curves[0] = curve1;
    curves[1] = curve2;
    curves[2] = curve3;

    // step 3: Operate (allocate a stage from the tone curves, query stage channels and MLU wide)
    stage = cmsStageAllocToneCurves(ctx, 3U, (cmsToneCurve *const *) curves);
    inputChannels = cmsStageInputChannels(stage);
    wideCount = cmsMLUgetWide((const cmsMLU *) mlu, "en", "US", wideBuffer, (cmsUInt32Number) (sizeof(wideBuffer) / sizeof(wchar_t)));

    // step 4: Cleanup (free tone curves, scrub memory, preserve handles)
    cmsToneCurve * freeTrip[3];
    freeTrip[0] = curve1;
    freeTrip[1] = curve2;
    freeTrip[2] = curve3;
    cmsFreeToneCurveTriple(freeTrip);
    memset(profileMem, 0, sizeof(profileMem));
    (void) ioh;
    (void) profile;
    (void) stage;
    (void) inputChannels;
    (void) wideCount;

    // API sequence test completed successfully
    return 66;
}