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
//<ID> 239
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    const cmsCIExyY * d50 = cmsD50_xyY();
    cmsHPROFILE labProfile = cmsCreateLab4Profile(d50);
    cmsHPROFILE bchProfile = cmsCreateBCHSWabstractProfile((cmsUInt32Number)33, (cmsFloat64Number)0.0, (cmsFloat64Number)0.0, (cmsFloat64Number)0.0, (cmsFloat64Number)0.0, (cmsUInt32Number)5000, (cmsUInt32Number)6500);
    cmsUInt32Number inFormat = (cmsUInt32Number)0;
    cmsUInt32Number outFormat = (cmsUInt32Number)0;
    cmsUInt32Number intent = (cmsUInt32Number)0;
    cmsUInt32Number flags = (cmsUInt32Number)0;
    cmsHTRANSFORM xform = cmsCreateTransform(labProfile, inFormat, bchProfile, outFormat, intent, flags);

    // step 2: Configure
    cmsUInt32Number bytesNeeded = (cmsUInt32Number)0;
    (void)cmsSaveProfileToMem(bchProfile, (void *)0, &bytesNeeded);
    void * mem = malloc((size_t)bytesNeeded);
    memset(mem, 0, (size_t)bytesNeeded);
    (void)cmsSaveProfileToMem(bchProfile, mem, &bytesNeeded);
    cmsCIEXYZ blackPoint;
    memset(&blackPoint, 0, sizeof(blackPoint));

    // step 3: Operate
    (void)cmsDetectDestinationBlackPoint(&blackPoint, bchProfile, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 4: Validate & Cleanup
    free(mem);
    (void)xform;
    cmsCloseProfile(bchProfile);
    cmsCloseProfile(labProfile);

    // API sequence test completed successfully
    return 66;
}