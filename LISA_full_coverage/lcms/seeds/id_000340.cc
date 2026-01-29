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
//<ID> 340
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations (Initialize)
    cmsHPROFILE srgb = cmsCreate_sRGBProfile();

    // Small CLUT table for a 2x2x2 grid, 3 input channels, 3 output channels -> 8 entries * 3 outputs = 24 floats
    const cmsFloat32Number clutTable[24] = {
        0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f
    };

    // step 2: Setup (Configure)
    cmsUInt32Number clutPoints[3] = { 2U, 2U, 2U };
    cmsUInt32Number inChan = 3U;
    cmsUInt32Number outChan = 3U;
    cmsStage * clutStage = cmsStageAllocCLutFloatGranular(NULL, clutPoints, inChan, outChan, clutTable);

    // Prepare a valid tone curve and a sample for evaluation
    const cmsFloat32Number tableValues[2] = { 0.0f, 1.0f };
    cmsToneCurve * curve = cmsBuildTabulatedToneCurveFloat(NULL, 2U, tableValues);
    const cmsToneCurve * ccurve = curve;
    cmsFloat32Number sample = 0.5f;

    // step 3: Operate (Operate & Validate)
    cmsFloat32Number evaluated = cmsEvalToneCurveFloat(ccurve, sample);
    cmsFloat32Number combined = evaluated + clutTable[0];
    (void) combined;
    (void) clutStage;

    // step 4: Cleanup
    cmsFreeToneCurve(curve);

    // API sequence test completed successfully
    return 66;
}