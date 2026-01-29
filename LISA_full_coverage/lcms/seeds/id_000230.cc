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
//<ID> 230
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    const cmsCIExyY * d50_ptr = cmsD50_xyY();
    cmsCIExyY whitePoint = *d50_ptr;
    cmsHPROFILE labProfile = cmsCreateLab4Profile(&whitePoint);
    cmsHPROFILE abstractProfile = cmsCreateBCHSWabstractProfile((cmsUInt32Number)33, (cmsFloat64Number)0.0, (cmsFloat64Number)1.0, (cmsFloat64Number)0.0, (cmsFloat64Number)1.0, (cmsUInt32Number)6500, (cmsUInt32Number)6500);
    cmsHTRANSFORM xform = cmsCreateTransform(labProfile, (cmsUInt32Number)0, abstractProfile, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 2: Configure
    cmsUInt32Number bytesNeeded = (cmsUInt32Number)0;
    cmsSaveProfileToMem(abstractProfile, NULL, &bytesNeeded);
    size_t allocSize = (size_t)(bytesNeeded + 16U);
    void * mem = malloc(allocSize);
    memset(mem, 0, allocSize);
    cmsSaveProfileToMem(abstractProfile, mem, &bytesNeeded);

    // step 3: Operate
    cmsCIEXYZ blackPoint;
    memset(&blackPoint, 0, sizeof(blackPoint));
    cmsDetectDestinationBlackPoint(&blackPoint, abstractProfile, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 4: Validate & Cleanup
    double bpX = blackPoint.X;
    double bpY = blackPoint.Y;
    double bpZ = blackPoint.Z;
    (void)bpX; (void)bpY; (void)bpZ;
    (void)xform;
    free(mem);
    cmsCloseProfile(labProfile);
    cmsCloseProfile(abstractProfile);

    // API sequence test completed successfully
    return 66;
}