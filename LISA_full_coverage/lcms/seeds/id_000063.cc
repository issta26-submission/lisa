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
//<ID> 63
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Initialization (use default NULL context)
    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    const cmsCIEXYZ * d50 = cmsD50_XYZ();
    cmsCIEXYZ destBlackPoint;
    memset(&destBlackPoint, 0, sizeof(destBlackPoint));

    // step 2: Configure / Operate
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    cmsBool detected = cmsDetectDestinationBlackPoint(&destBlackPoint, hProfile, 0U, 0U);

    // step 3: Validate / Transform
    cmsUInt16Number encXYZ[3];
    memset(encXYZ, 0, sizeof(encXYZ));
    cmsFloat2XYZEncoded(encXYZ, &destBlackPoint);
    cmsFloat64Number version = cmsGetProfileVersion(hProfile);

    // step 4: Cleanup
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);

    (void) d50;
    (void) tagCount;
    (void) detected;
    (void) destBlackPoint;
    (void) encXYZ;
    (void) version;

    // API sequence test completed successfully
    return 66;
}