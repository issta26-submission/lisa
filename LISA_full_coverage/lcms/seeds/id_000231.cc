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
//<ID> 231
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    const cmsCIExyY * d50 = cmsD50_xyY();
    cmsHPROFILE lab4 = cmsCreateLab4Profile(d50);
    cmsHPROFILE abstractProfile = cmsCreateBCHSWabstractProfile((cmsUInt32Number)17, (cmsFloat64Number)0.5, (cmsFloat64Number)1.2, (cmsFloat64Number)0.0, (cmsFloat64Number)1.0, (cmsUInt32Number)6500, (cmsUInt32Number)5000);
    cmsUInt32Number inFormat = (cmsUInt32Number)0;
    cmsUInt32Number outFormat = (cmsUInt32Number)0;
    cmsUInt32Number intent = (cmsUInt32Number)0;
    cmsUInt32Number flags = (cmsUInt32Number)0;

    // step 2: Configure
    cmsHTRANSFORM transform = cmsCreateTransform(lab4, inFormat, abstractProfile, outFormat, intent, flags);

    // step 3: Operate
    cmsHPROFILE devlink = cmsTransform2DeviceLink(transform, (cmsFloat64Number)4.5, (cmsUInt32Number)0);
    cmsCIEXYZ blackPoint;
    memset(&blackPoint, 0, sizeof(blackPoint));
    cmsDetectDestinationBlackPoint(&blackPoint, devlink, intent, flags);
    cmsUInt32Number bytesNeeded = (cmsUInt32Number)0;
    cmsSaveProfileToMem(devlink, (void *)NULL, &bytesNeeded);
    void * mem = malloc((size_t)bytesNeeded + 1);
    memset(mem, 0, (size_t)bytesNeeded + 1);
    cmsSaveProfileToMem(devlink, mem, &bytesNeeded);

    // step 4: Validate & Cleanup
    free(mem);
    cmsCloseProfile(devlink);
    cmsCloseProfile(abstractProfile);
    cmsCloseProfile(lab4);

    // API sequence test completed successfully
    (void)transform;
    return 66;
}