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
//<ID> 217
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = (cmsContext)NULL;

    cmsToneCurve * gamma = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    cmsBool linear = cmsIsToneCurveLinear(gamma);
    cmsBool descending = cmsIsToneCurveDescending(gamma);
    cmsFloat64Number estimated = cmsEstimateGamma(gamma, (cmsFloat64Number)1e-6);
    const cmsCurveSegment * segment = cmsGetToneCurveSegment(0, gamma);
    int paramType = cmsGetToneCurveParametricType(gamma);

    cmsToneCurve * triple[3];
    triple[0] = gamma;
    triple[1] = NULL;
    triple[2] = NULL;
    cmsFreeToneCurveTriple(triple);

    cmsStage * stage = cmsStageAllocIdentity(ctx, (cmsUInt32Number)3);
    cmsStage * dup = cmsStageDup(stage);
    cmsStage * next = cmsStageNext(stage);

    cmsUInt32Number clutPoints[3] = { 2, 2, 2 };
    cmsStage * clut = cmsStageAllocCLutFloat(ctx, (cmsUInt32Number)2, (cmsUInt32Number)3, (cmsUInt32Number)3, (const cmsFloat32Number *)NULL);
    cmsStage * clutGran = cmsStageAllocCLutFloatGranular(ctx, clutPoints, (cmsUInt32Number)3, (cmsUInt32Number)3, (const cmsFloat32Number *)NULL);

    cmsUInt32Number outChannels = cmsStageOutputChannels(dup);

    (void)linear;
    (void)descending;
    (void)estimated;
    (void)segment;
    (void)paramType;
    (void)stage;
    (void)next;
    (void)clut;
    (void)clutGran;
    (void)outChannels;

    return 66;
}