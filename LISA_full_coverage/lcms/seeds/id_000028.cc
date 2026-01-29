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
//<ID> 28
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Step 1: Declarations
    cmsContext ctx = (cmsContext) NULL;
    cmsIOHANDLER * io = cmsOpenIOhandlerFromNULL(ctx);
    unsigned char profileMem[128];
    cmsHPROFILE profile = (cmsHPROFILE) NULL;
    cmsToneCurve * curves[3];
    cmsStage * stage = (cmsStage *) NULL;
    cmsUInt32Number inChannels = 0;
    wchar_t wideBuffer[64];
    const char lang[3] = { 'e', 'n', 'g' };
    const char country[3] = { 'U', 'S', 'A' };
    cmsUInt32Number wideLen = 0;
    cmsFloat64Number params[1];
    params[0] = 2.2;

    // Step 2: Setup
    memset(profileMem, 0, sizeof(profileMem));
    profile = cmsOpenProfileFromMem(profileMem, (cmsUInt32Number) sizeof(profileMem));
    curves[0] = cmsBuildParametricToneCurve(ctx, 1, params);
    curves[1] = cmsBuildParametricToneCurve(ctx, 1, params);
    curves[2] = cmsBuildParametricToneCurve(ctx, 1, params);
    stage = cmsStageAllocToneCurves(ctx, 3U, (cmsToneCurve * const *) curves);

    // Step 3: Core operations
    inChannels = cmsStageInputChannels(stage);
    memset(wideBuffer, 0, sizeof(wideBuffer));
    wideLen = cmsMLUgetWide((const cmsMLU *) NULL, lang, country, wideBuffer, (cmsUInt32Number) (sizeof(wideBuffer) / sizeof(wideBuffer[0])));

    // Step 4: Cleanup
    cmsFreeToneCurveTriple(curves);

    (void) io;
    (void) profile;
    (void) stage;
    (void) inChannels;
    (void) wideLen;

    // API sequence test completed successfully
    return 66;
}