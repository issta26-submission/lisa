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
//<ID> 178
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsHPROFILE hProfile = cmsCreateNULLProfile();
    cmsContext ctx = cmsGetProfileContextID(hProfile);
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    cmsUInt32Number tagSig = 0x74657374U; /* 'test' */
    unsigned char rawData[16];
    memset(rawData, 0, sizeof(rawData));
    rawData[0] = 'L';
    rawData[1] = 'C';
    rawData[2] = 'M';
    rawData[3] = 'S';
    cmsFloat64Number newVersion = 4.50;

    // step 2: Configure
    cmsIT8SetPropertyMulti(hIT8, "Device", "Manufacturer", "ACME Inc.");
    const char * manuf = cmsIT8GetPropertyMulti(hIT8, "Device", "Manufacturer");
    cmsSetProfileVersion(hProfile, newVersion);
    cmsSetHeaderModel(hProfile, tagSig);

    // step 3: Operate
    cmsBool wrote = cmsWriteRawTag(hProfile, (cmsTagSignature) tagSig, rawData, (cmsUInt32Number) sizeof(rawData));

    // step 4: Validate & Cleanup
    cmsIT8Free(hIT8);

    (void)ctx;
    (void)manuf;
    (void)wrote;
    (void)rawData;
    (void)hProfile;
    (void)tagSig;
    (void)newVersion;

    // API sequence test completed successfully
    return 66;
}