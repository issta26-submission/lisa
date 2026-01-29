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
//<ID> 216
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = (cmsContext)NULL;
    cmsToneCurve * curves[3];
    curves[0] = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    curves[1] = cmsBuildGamma(ctx, (cmsFloat64Number)1.8);
    curves[2] = cmsBuildGamma(ctx, (cmsFloat64Number)1.0);
    cmsStage * stage = cmsStageAllocIdentity(ctx, (cmsUInt32Number)3);
    cmsNAMEDCOLORLIST * ncl = cmsAllocNamedColorList(ctx, (cmsUInt32Number)4, (cmsUInt32Number)3, "Pfx", "Sfx");
    cmsHANDLE dict = cmsDictAlloc(ctx);
    const cmsDICTentry * entries = cmsDictGetEntryList(dict);
    cmsPipeline * lut = (cmsPipeline *)NULL;

    // step 2: Configure
    cmsBool monotonic = cmsIsToneCurveMonotonic(curves[0]);
    (void)monotonic;

    // step 3: Operate
    void * stageData = cmsStageData(stage);
    cmsUInt32Number estimated = cmsGetToneCurveEstimatedTableEntries(curves[0]);
    (void)stageData;
    (void)estimated;
    (void)entries;

    // step 4: Validate & Cleanup
    cmsFreeToneCurveTriple(curves);
    cmsStageFree(stage);
    cmsPipelineFree(lut);
    cmsDictFree(dict);
    cmsFreeNamedColorList(ncl);
    (void)ctx;
    (void)curves;
    (void)stage;
    (void)ncl;
    (void)dict;
    (void)entries;
    (void)lut;
    // API sequence test completed successfully
    return 66;
}