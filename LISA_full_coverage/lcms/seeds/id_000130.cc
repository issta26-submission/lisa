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
//<ID> 130
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsHPROFILE hProfile2 = NULL;
    cmsToneCurve * t1 = NULL;
    cmsToneCurve * t2 = NULL;
    cmsToneCurve * t3 = NULL;
    cmsToneCurve * dup = NULL;
    cmsToneCurve * rev = NULL;
    cmsToneCurve * arr[3];
    int ver = 0;
    double est = 0.0;
    cmsColorSpaceSignature cs = _cmsICCcolorSpace(0);

    t1 = cmsBuildGamma(ctx, 2.2);
    t2 = cmsBuildGamma(ctx, 1.8);
    t3 = cmsBuildGamma(ctx, 2.0);

    arr[0] = t1;
    arr[1] = t2;
    arr[2] = t3;

    hProfile = cmsCreateLinearizationDeviceLink(cs, arr);
    hProfile2 = cmsCreateLinearizationDeviceLinkTHR(ctx, cs, arr);

    dup = cmsDupToneCurve(t2);
    rev = cmsReverseToneCurveEx((cmsUInt32Number)256, t1);

    (void)cmsIsToneCurveLinear(t1);
    (void)cmsIsToneCurveDescending(t2);
    (void)cmsIsToneCurveMultisegment(t3);

    est = cmsEstimateGamma(t1, 0.0001);
    ver = cmsGetEncodedCMMversion();

    (void)hProfile;
    (void)hProfile2;
    (void)dup;
    (void)rev;
    (void)ver;
    (void)est;
    (void)ctx;

    return 66;
}