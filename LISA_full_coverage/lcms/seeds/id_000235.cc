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
//<ID> 235
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsHPROFILE hBCHSW = cmsCreateBCHSWabstractProfile((cmsUInt32Number)17, (cmsFloat64Number)50.0, (cmsFloat64Number)1.0, (cmsFloat64Number)0.0, (cmsFloat64Number)1.0, (cmsUInt32Number)3000, (cmsUInt32Number)5000);
    const cmsCIExyY * whitePoint = cmsD50_xyY();
    cmsHPROFILE hLab4 = cmsCreateLab4Profile(whitePoint);
    cmsHTRANSFORM xform = cmsCreateTransform(hLab4, (cmsUInt32Number)0, hBCHSW, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsCIEXYZ blackPoint;
    memset(&blackPoint, 0, sizeof(blackPoint));
    cmsUInt32Number bytesNeeded = (cmsUInt32Number)0;
    cmsSaveProfileToMem(hBCHSW, (void *)NULL, &bytesNeeded);
    cmsUInt32Number allocSize = bytesNeeded + (cmsUInt32Number)1;
    void * mem = malloc((size_t)allocSize);
    memset(mem, 0, (size_t)allocSize);
    cmsSaveProfileToMem(hBCHSW, mem, &bytesNeeded);

    // step 2: Configure
    cmsDetectDestinationBlackPoint(&blackPoint, hBCHSW, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 3: Operate
    unsigned char inputPixel[4];
    unsigned char outputPixel[4];
    memset(inputPixel, 0, sizeof(inputPixel));
    inputPixel[0] = 0;
    inputPixel[1] = 128;
    inputPixel[2] = 255;
    inputPixel[3] = 0;
    memset(outputPixel, 0, sizeof(outputPixel));
    cmsDoTransform(xform, (const void *)inputPixel, (void *)outputPixel, (cmsUInt32Number)1);

    // step 4: Validate & Cleanup
    free(mem);
    cmsCloseProfile(hBCHSW);
    cmsCloseProfile(hLab4);

    // API sequence test completed successfully
    return 66;
}