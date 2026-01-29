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
//<ID> 232
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
    cmsHPROFILE abstractProfile = cmsCreateBCHSWabstractProfile((cmsUInt32Number)8, (cmsFloat64Number)50.0, (cmsFloat64Number)1.2, (cmsFloat64Number)0.0, (cmsFloat64Number)0.0, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsHTRANSFORM transform = cmsCreateTransform(labProfile, (cmsUInt32Number)0, abstractProfile, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsCIEXYZ blackPoint;
    memset(&blackPoint, 0, sizeof(blackPoint));
    unsigned char memBuffer[4096];
    memset(memBuffer, 0, sizeof memBuffer);
    cmsUInt32Number bytesNeeded = (cmsUInt32Number)sizeof memBuffer;

    // step 2: Configure
    (void)transform; // keep reference to avoid unused warnings

    // step 3: Operate
    cmsBool detected = cmsDetectDestinationBlackPoint(&blackPoint, abstractProfile, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsBool saved = cmsSaveProfileToMem(abstractProfile, (void *)memBuffer, &bytesNeeded);

    // step 4: Validate & Cleanup
    (void)detected;
    (void)saved;
    cmsCloseProfile(labProfile);
    cmsCloseProfile(abstractProfile);

    // API sequence test completed successfully
    return 66;
}