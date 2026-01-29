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
//<ID> 236
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsHPROFILE srcProfile = cmsCreateBCHSWabstractProfile((cmsUInt32Number)33, (cmsFloat64Number)0.5, (cmsFloat64Number)1.1, (cmsFloat64Number)0.0, (cmsFloat64Number)1.0, (cmsUInt32Number)5000, (cmsUInt32Number)6500);
    const cmsCIExyY * d50 = cmsD50_xyY();
    cmsHPROFILE dstProfile = cmsCreateLab4Profile(d50);
    cmsUInt32Number inputFormat = (cmsUInt32Number)0x00010001U;
    cmsUInt32Number outputFormat = (cmsUInt32Number)0x00020002U;
    cmsUInt32Number intent = (cmsUInt32Number)0;
    cmsUInt32Number flags = (cmsUInt32Number)0;

    // step 2: Configure
    cmsHTRANSFORM transform = cmsCreateTransform(srcProfile, inputFormat, dstProfile, outputFormat, intent, flags);

    // step 3: Operate
    cmsCIEXYZ blackPoint;
    memset(&blackPoint, 0, sizeof(blackPoint));
    cmsBool detected = cmsDetectDestinationBlackPoint(&blackPoint, dstProfile, intent, flags);
    cmsUInt32Number bytesNeeded = 0;
    cmsBool queryOk = cmsSaveProfileToMem(srcProfile, (void *)NULL, &bytesNeeded);
    unsigned char * memBuffer = (unsigned char *)operator new[]( (size_t)(bytesNeeded | 1) );
    memset(memBuffer, 0, (size_t)(bytesNeeded | 1));
    cmsBool saveOk = cmsSaveProfileToMem(srcProfile, (void *)memBuffer, &bytesNeeded);

    // step 4: Validate & Cleanup
    (void)transform;
    (void)detected;
    (void)queryOk;
    (void)saveOk;
    (void)bytesNeeded;
    (void)blackPoint;
    cmsCloseProfile(srcProfile);
    cmsCloseProfile(dstProfile);
    operator delete[](memBuffer);

    return 66;
    // API sequence test completed successfully
}