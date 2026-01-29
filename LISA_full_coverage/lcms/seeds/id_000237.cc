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
//<ID> 237
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    const cmsCIExyY * whitePoint = cmsD50_xyY();
    cmsHPROFILE inputProfile = cmsCreateBCHSWabstractProfile((cmsUInt32Number)17, (cmsFloat64Number)50.0, (cmsFloat64Number)10.0, (cmsFloat64Number)0.0, (cmsFloat64Number)1.0, (cmsUInt32Number)5000, (cmsUInt32Number)6500);
    cmsHPROFILE outputProfile = cmsCreateLab4Profile(whitePoint);
    cmsHTRANSFORM transform = cmsCreateTransform(inputProfile, (cmsUInt32Number)0, outputProfile, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 2: Configure
    cmsCIEXYZ blackPoint;
    memset((void *)&blackPoint, 0, sizeof(blackPoint));
    cmsDetectDestinationBlackPoint(&blackPoint, outputProfile, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 3: Operate
    cmsUInt32Number bytesNeeded = 0;
    cmsSaveProfileToMem(outputProfile, NULL, &bytesNeeded);
    cmsUInt32Number bufferSize = bytesNeeded + 1;
    void * mem = malloc((size_t)bufferSize);
    memset(mem, 0, (size_t)bufferSize);
    cmsBool saved = cmsSaveProfileToMem(outputProfile, mem, &bytesNeeded);
    cmsHPROFILE devLink = cmsTransform2DeviceLink(transform, (cmsFloat64Number)4.3, (cmsUInt32Number)0);

    // step 4: Validate & Cleanup
    cmsCloseProfile(inputProfile);
    cmsCloseProfile(outputProfile);
    cmsCloseProfile(devLink);
    free(mem);
    (void)saved;
    (void)transform;
    (void)blackPoint;
    (void)whitePoint;
    (void)bytesNeeded;

    // API sequence test completed successfully
    return 66;
}