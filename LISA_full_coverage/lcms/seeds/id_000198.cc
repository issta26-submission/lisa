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
//<ID> 198
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsHANDLE hIT8 = cmsIT8Alloc((cmsContext)NULL);
    unsigned char memProfileData[64];
    memset(memProfileData, 0, sizeof(memProfileData));
    memProfileData[0] = 'a';
    memProfileData[1] = 'c';
    memProfileData[2] = 'm';
    memProfileData[3] = 'e';
    cmsHPROFILE hMemProfile = cmsCreate_sRGBProfile();
    FILE * stream = tmpfile();
    cmsHPROFILE hStreamProfile = cmsCreateXYZProfile();
    cmsHPROFILE hNullProfile = cmsCreateNULLProfile();

    // step 2: Configure
    cmsUInt32Number headerManufacturer = (cmsUInt32Number)0x4D4F444CU; /* 'MODL' */
    cmsSetHeaderManufacturer(hNullProfile, headerManufacturer);

    // step 3: Operate
    cmsHTRANSFORM hTransform = cmsCreateTransform(hMemProfile, (cmsUInt32Number)0, hNullProfile, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsUInt32Number inFormat = cmsGetTransformInputFormat(hTransform);

    // step 4: Validate & Cleanup
    cmsCloseProfile(hMemProfile);
    cmsCloseProfile(hStreamProfile);
    cmsCloseProfile(hNullProfile);
    fclose(stream);

    (void)inFormat;
    (void)headerManufacturer;
    (void)hIT8;
    (void)memProfileData;
    (void)hMemProfile;
    (void)hStreamProfile;
    (void)hNullProfile;
    (void)hTransform;
    // API sequence test completed successfully
    return 66;
}