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
//<ID> 348
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
    cmsNAMEDCOLORLIST * ncl = cmsAllocNamedColorList(ctx, (cmsUInt32Number)2, (cmsUInt32Number)3, "Pref", "Suf");
    cmsUInt32Number nGrid = (cmsUInt32Number)2;
    cmsUInt32Number inCh = (cmsUInt32Number)3;
    cmsUInt32Number outCh = (cmsUInt32Number)3;
    cmsFloat32Number table[8 * 3]; // nGrid^inCh = 2^3 = 8, times outCh = 3

    // step 2: Setup
    memset(table, 0, sizeof(table));
    cmsUInt16Number gamma_vals[2] = { 0, 65535 };
    cmsToneCurve * gamma = cmsBuildTabulatedToneCurve16(ctx, 2, gamma_vals);
    cmsFloat32Number toneSample = cmsEvalToneCurveFloat(gamma, (cmsFloat32Number)0.5f);
    table[0] = toneSample;

    // step 3: Core operations
    cmsStage * stage = cmsStageAllocCLutFloat(ctx, nGrid, inCh, outCh, table);
    (void) stage;
    (void) toneSample;

    // step 4: Cleanup
    cmsFreeToneCurve(gamma);
    cmsFreeNamedColorList(ncl);
    cmsCloseProfile(srgb);
    cmsDeleteContext(ctx);

    // API sequence test completed successfully
    return 66;
}