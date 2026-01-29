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
//<ID> 233
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsCIExyY whitePoint;
    memset(&whitePoint, 0, sizeof(whitePoint));
    whitePoint.x = (cmsFloat64Number)0.3457;
    whitePoint.y = (cmsFloat64Number)0.3585;
    whitePoint.Y = (cmsFloat64Number)1.0;
    cmsHPROFILE labProfile = cmsCreateLab4Profile(&whitePoint);
    cmsHPROFILE abstractProfile = cmsCreateBCHSWabstractProfile((cmsUInt32Number)16U, (cmsFloat64Number)0.0, (cmsFloat64Number)1.0, (cmsFloat64Number)0.0, (cmsFloat64Number)1.0, (cmsUInt32Number)6500U, (cmsUInt32Number)6500U);
    cmsHTRANSFORM transform = cmsCreateTransform(labProfile, (cmsUInt32Number)0U, abstractProfile, (cmsUInt32Number)0U, (cmsUInt32Number)0U, (cmsUInt32Number)0U);

    // step 2: Configure
    cmsCIEXYZ blackPoint;
    memset(&blackPoint, 0, sizeof(blackPoint));
    cmsBool blackDetected = cmsDetectDestinationBlackPoint(&blackPoint, abstractProfile, (cmsUInt32Number)0U, (cmsUInt32Number)0U);

    // step 3: Operate
    cmsUInt32Number bytesNeeded = (cmsUInt32Number)0U;
    cmsBool ok = cmsSaveProfileToMem(abstractProfile, (void *)NULL, &bytesNeeded);
    cmsUInt32Number allocSize = (bytesNeeded > 0U) ? bytesNeeded : (cmsUInt32Number)1U;
    void * mem = malloc((size_t)allocSize);
    if (mem) {
        memset(mem, 0, (size_t)allocSize);
        ok = cmsSaveProfileToMem(abstractProfile, mem, &bytesNeeded);
    }

    // step 4: Validate & Cleanup
    if (mem) { free(mem); }
    cmsCloseProfile(labProfile);
    cmsCloseProfile(abstractProfile);
    (void)transform;
    (void)blackDetected;
    (void)blackPoint;
    (void)ok;
    // API sequence test completed successfully
    return 66;
}