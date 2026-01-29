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
//<ID> 238
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsHPROFILE hBCH = cmsCreateBCHSWabstractProfile((cmsUInt32Number)33, (cmsFloat64Number)0.10, (cmsFloat64Number)1.20, (cmsFloat64Number)0.0, (cmsFloat64Number)1.0, (cmsUInt32Number)6500, (cmsUInt32Number)5000);
    const cmsCIExyY * d50 = cmsD50_xyY();
    cmsHPROFILE hLab4 = cmsCreateLab4Profile(d50);
    cmsUInt32Number inputFormat  = (cmsUInt32Number)0x42434853U; // 'BCHS' (arbitrary packed code)
    cmsUInt32Number outputFormat = (cmsUInt32Number)0x4C413442U; // 'LA4B' (arbitrary packed code)
    cmsHTRANSFORM hTransform = cmsCreateTransform(hBCH, inputFormat, hLab4, outputFormat, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsCIEXYZ blackPoint;
    memset(&blackPoint, 0, sizeof(blackPoint));
    cmsUInt32Number bytesNeeded = (cmsUInt32Number)0;
    void * mem = (void *)0;

    // step 2: Configure
    cmsSaveProfileToMem(hBCH, NULL, &bytesNeeded);
    mem = malloc((size_t)(bytesNeeded + (cmsUInt32Number)16));
    memset(mem, 0, (size_t)(bytesNeeded + (cmsUInt32Number)16));

    // step 3: Operate
    cmsSaveProfileToMem(hBCH, mem, &bytesNeeded);
    cmsDetectDestinationBlackPoint(&blackPoint, hLab4, (cmsUInt32Number)0, (cmsUInt32Number)0);
    volatile cmsFloat64Number blackSum = blackPoint.X + blackPoint.Y + blackPoint.Z;
    (void)blackSum;

    // step 4: Validate & Cleanup
    free(mem);
    cmsCloseProfile(hBCH);
    cmsCloseProfile(hLab4);
    (void)hTransform;

    // API sequence test completed successfully
    return 66;
}