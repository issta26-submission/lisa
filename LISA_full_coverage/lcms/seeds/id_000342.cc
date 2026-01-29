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
//<ID> 342
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE srgb = cmsCreate_sRGBProfileTHR(ctx);
    cmsNAMEDCOLORLIST * named = cmsAllocNamedColorList(ctx, (cmsUInt32Number)1, (cmsUInt32Number)3, "Pfx", "Sfx");

    // step 2: Setup
    static const cmsFloat32Number clutTable[24] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f
    };
    cmsStage * stage = cmsStageAllocCLutFloat(ctx, (cmsUInt32Number)2, (cmsUInt32Number)3, (cmsUInt32Number)3, clutTable);

    // step 3: Core operations
    cmsToneCurve * gamma = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    cmsFloat32Number sample = cmsEvalToneCurveFloat(gamma, (cmsFloat32Number)0.5f);
    cmsStageSignature stype = cmsStageType(stage);
    cmsUInt32Number outCh = cmsStageOutputChannels(stage);
    char nameBuf[64];
    char pfxBuf[64];
    char sfxBuf[64];
    cmsUInt16Number pcs[8];
    cmsUInt16Number colorant[8];
    memset(nameBuf, 0, sizeof(nameBuf));
    memset(pfxBuf, 0, sizeof(pfxBuf));
    memset(sfxBuf, 0, sizeof(sfxBuf));
    (void) cmsNamedColorInfo(named, (cmsUInt32Number)0, nameBuf, pfxBuf, sfxBuf, pcs, colorant);
    (void) sample;
    (void) stype;
    (void) outCh;

    // step 4: Cleanup
    cmsFreeToneCurve(gamma);
    cmsFreeNamedColorList(named);
    cmsCloseProfile(srgb);
    return 66;
    // API sequence test completed successfully
}