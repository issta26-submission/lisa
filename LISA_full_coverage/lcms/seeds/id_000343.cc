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
//<ID> 343
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsToneCurve * gamma = cmsBuildGamma((cmsContext)NULL, (cmsFloat64Number)2.2);
    cmsFloat32Number eval = cmsEvalToneCurveFloat(gamma, (cmsFloat32Number)0.5f);
    cmsBool linear = cmsIsToneCurveLinear(gamma);
    cmsBool monotonic = cmsIsToneCurveMonotonic(gamma);
    cmsBool descending = cmsIsToneCurveDescending(gamma);
    cmsFloat64Number prevAdapt = cmsSetAdaptationState((cmsFloat64Number)1.0);

    int cmmver = cmsGetEncodedCMMversion();

    cmsCIELab lab;
    lab.L = 50.0f;
    lab.a = 10.0f;
    lab.b = -5.0f;

    cmsCIELCh lch;
    cmsLab2LCh(&lch, &lab);

    cmsUInt16Number encoded[3];
    cmsFloat2LabEncoded(encoded, &lab);
    cmsFloat2LabEncodedV2(encoded, &lab);

    cmsFloat32Number metric = eval + (cmsFloat32Number)encoded[0] + (cmsFloat32Number)linear + (cmsFloat32Number)monotonic + (cmsFloat32Number)descending + (cmsFloat32Number)prevAdapt + (cmsFloat32Number)cmmver;
    (void)metric;

    return 66;
}