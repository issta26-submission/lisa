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
//<ID> 207
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Create an sRGB profile in the default (NULL) context
    cmsHPROFILE hProfile = cmsCreate_sRGBProfileTHR(NULL);
    if (!hProfile) return 1;

    // Allocate an IT8 handle and set a simple data format
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    if (hIT8) {
        cmsBool setFormatOk = cmsIT8SetDataFormat(hIT8, 0, "R,G,B");
        (void)setFormatOk;
    }

    // Build a simple 2-entry tabulated tone curve and evaluate it
    cmsUInt16Number table[2] = { 0, 65535 };
    cmsToneCurve * curve = cmsBuildTabulatedToneCurve16(NULL, 2, table);
    cmsFloat32Number eval = 0.0f;
    if (curve) {
        eval = cmsEvalToneCurveFloat(curve, 0.5f);
    }

    // Obtain the profile context and try to register a (NULL) plugin using THR API
    cmsContext ctx = cmsGetProfileContextID(hProfile);
    cmsBool pluginResult = cmsPluginTHR(ctx, (void *)NULL);
    (void)pluginResult;

    // Query a tag signature from the profile and check if that tag exists
    cmsTagSignature sig = cmsGetTagSignature(hProfile, 0);
    cmsBool hasTag = cmsIsTag(hProfile, sig);
    (void)hasTag;

    // Create a second sRGB profile and a transform between them, then get its output format
    cmsHPROFILE hProfileOut = cmsCreate_sRGBProfileTHR(NULL);
    cmsHTRANSFORM hTransform = cmsCreateTransform(hProfile, 0, hProfileOut, 0, 0, 0);
    cmsUInt32Number outFormat = 0;
    if (hTransform) {
        outFormat = cmsGetTransformOutputFormat(hTransform);
    }
    (void)outFormat;

    // Clean up profiles
    cmsCloseProfile(hProfile);
    cmsCloseProfile(hProfileOut);

    // Indicate success as requested
    return 66;
}