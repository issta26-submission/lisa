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
//<ID> 213
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Step 1: Create a context and some tone curves
    cmsContext ctx = cmsCreateContext(NULL, NULL);

    cmsUInt16Number tableVals[4] = {0, 21845, 43690, 65535};
    cmsToneCurve * curveR = cmsBuildTabulatedToneCurve16(ctx, 4, tableVals);
    cmsToneCurve * curveG = cmsBuildTabulatedToneCurve16(ctx, 4, tableVals);
    cmsToneCurve * curveB = cmsBuildTabulatedToneCurve16(ctx, 4, tableVals);
    cmsToneCurve * curves[3];
    curves[0] = curveR;
    curves[1] = curveG;
    curves[2] = curveB;

    // Step 2: Allocate stages (identity and tone-curves stage) and duplicate one stage
    cmsStage * identity = cmsStageAllocIdentity(ctx, (cmsUInt32Number)3);
    cmsStage * toneStage = cmsStageAllocToneCurves(ctx, (cmsUInt32Number)3, curves);
    cmsStage * dupStage = cmsStageDup(toneStage);

    // Step 3: Query stage contexts and tweak adaptation state
    cmsContext idCtx = cmsGetStageContextID(identity);
    cmsContext dupCtx = cmsGetStageContextID(dupStage);
    (void) idCtx;
    (void) dupCtx;

    cmsFloat64Number prevAdapt = cmsSetAdaptationState(1.0);
    (void) prevAdapt;

    // Step 4: Encode/decode Lab values and compute deltaE
    cmsCIELab labA;
    memset(&labA, 0, sizeof(labA));
    cmsUInt16Number encodedLab[3];
    cmsFloat2LabEncodedV2(encodedLab, &labA);

    cmsCIELab labB;
    cmsLabEncoded2FloatV2(&labB, encodedLab);

    cmsFloat64Number deltaE = cmsCIE2000DeltaE(&labA, &labB, 1.0, 1.0, 1.0);
    (void) deltaE;

    // Step 5: Create a profile, inspect and modify headers, and save via IO handler/stream
    cmsHPROFILE srgb = cmsCreate_sRGBProfile();
    cmsUInt32Number headerModel = cmsGetHeaderModel(srgb);
    (void) headerModel;

    cmsSetHeaderRenderingIntent(srgb, (cmsUInt32Number)0);

    cmsUInt64Number headerFlags = 0;
    cmsGetHeaderAttributes(srgb, &headerFlags);
    (void) headerFlags;

    cmsUInt8Number profileID[16];
    cmsGetHeaderProfileID(srgb, profileID);

    struct tm creation;
    cmsGetHeaderCreationDateTime(srgb, &creation);

    FILE * tmp = tmpfile();
    cmsIOHANDLER * io = cmsOpenIOhandlerFromStream(ctx, tmp);
    cmsUInt32Number bytesWritten = cmsSaveProfileToIOhandler(srgb, io);
    (void) bytesWritten;

    // Also attempt direct stream save (may be redundant but exercises API)
    cmsSaveProfileToStream(srgb, tmp);

    // Avoid unused-variable warnings for allocated objects we didn't free (no free APIs provided above)
    (void) curveR;
    (void) curveG;
    (void) curveB;
    (void) curves;
    (void) identity;
    (void) toneStage;
    (void) dupStage;
    (void) srgb;
    (void) io;
    (void) tmp;
    (void) encodedLab;
    (void) profileID;
    (void) creation;

    // API sequence test completed successfully
    return 66;
}