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
//<ID> 75
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsDICTentry dictEntry;
    memset(&dictEntry, 0, sizeof(dictEntry));

    unsigned char profMem[64];
    memset(profMem, 0, sizeof(profMem));
    cmsHPROFILE hProfile = cmsOpenProfileFromMem((const void *) profMem, (cmsUInt32Number) sizeof(profMem));

    cmsHPROFILE pNull = cmsCreateNULLProfile();
    cmsHPROFILE pXYZ = cmsCreateXYZProfile();
    const cmsCIExyY * d50 = cmsD50_xyY();
    cmsHPROFILE pRGB = cmsCreateRGBProfile(d50, NULL, NULL);

    cmsUInt32Number headerIntent = cmsGetHeaderRenderingIntent(pNull);
    cmsColorSpaceSignature colorSpace = cmsGetColorSpace(pRGB);
    cmsUInt32Number pcs = cmsGetPCS(pNull);

    cmsToneCurve * tone = cmsBuildGamma((cmsContext) NULL, (cmsFloat64Number) 2.2);
    cmsBool isLinear = cmsIsToneCurveLinear(tone);
    cmsUInt16Number eval16 = cmsEvalToneCurve16(tone, (cmsUInt16Number) 32768U);
    cmsFloat32Number evalFloat = cmsEvalToneCurveFloat(tone, (cmsFloat32Number) 0.5f);
    cmsFloat64Number est = cmsEstimateGamma(tone, (cmsFloat64Number) 0.001);

    cmsToneCurve * rev = cmsReverseToneCurve(tone);
    cmsInt32Number paramType = cmsGetToneCurveParametricType(tone);
    const cmsCurveSegment * seg = cmsGetToneCurveSegment(0, tone);

    void * tag = cmsReadTag(pRGB, (cmsTagSignature) 0);
    cmsUInt32Number rawSize = cmsReadRawTag(pRGB, (cmsTagSignature) 0, NULL, (cmsUInt32Number) 0);

    (void) dictEntry;
    (void) hProfile;
    (void) pNull;
    (void) pXYZ;
    (void) pRGB;
    (void) headerIntent;
    (void) colorSpace;
    (void) pcs;
    (void) tone;
    (void) isLinear;
    (void) eval16;
    (void) evalFloat;
    (void) est;
    (void) rev;
    (void) paramType;
    (void) seg;
    (void) tag;
    (void) rawSize;

    return 66;
}