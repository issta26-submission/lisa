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
//<ID> 347
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations / Initialize
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE srgb = cmsCreate_sRGBProfileTHR(ctx);
    cmsToneCurve * gamma = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    cmsNAMEDCOLORLIST * ncl = cmsAllocNamedColorList(ctx, (cmsUInt32Number)1, (cmsUInt32Number)3, "Prefix", "Suffix");

    // step 2: Configure resources (CLUT stage)
    cmsUInt32Number nGridPoints = (cmsUInt32Number)2;
    cmsUInt32Number inChannels = (cmsUInt32Number)3;
    cmsUInt32Number outChannels = (cmsUInt32Number)3;
    const cmsFloat32Number clutTable[24] = {
        0.0f, 0.1f, 0.2f,   0.3f, 0.4f, 0.5f,
        0.6f, 0.7f, 0.8f,   0.9f, 1.0f, 1.1f,
        1.2f, 1.3f, 1.4f,   1.5f, 1.6f, 1.7f,
        1.8f, 1.9f, 2.0f,   2.1f, 2.2f, 2.3f
    };
    cmsStage * clutStage = cmsStageAllocCLutFloat(ctx, nGridPoints, inChannels, outChannels, clutTable);

    // step 3: Operate & Validate
    cmsFloat32Number sample = clutTable[0];
    cmsFloat32Number evaluated = cmsEvalToneCurveFloat(gamma, sample);
    cmsUInt32Number stageOut = cmsStageOutputChannels(clutStage);
    (void) evaluated;
    (void) stageOut;

    // step 4: Cleanup
    cmsFreeToneCurve(gamma);
    cmsFreeNamedColorList(ncl);
    cmsCloseProfile(srgb);

    return 66;
    // API sequence test completed successfully
}