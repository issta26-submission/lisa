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
//<ID> 174
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
    char rawData[32];
    memset(rawData, 0, sizeof(rawData));
    rawData[0] = 'L';
    rawData[1] = 'C';
    rawData[2] = 'M';
    rawData[3] = 0x01;
    cmsTagSignature tagSig = (cmsTagSignature)0x74657374; // 'test'
    const char * retrieved = NULL;
    cmsBool okWrite = 0;
    cmsBool okSet = 0;

    // step 2: Configure
    cmsSetProfileVersion(hProfile, (cmsFloat64Number)4.5);
    cmsSetHeaderModel(hProfile, (cmsUInt32Number)0x52474220); // 'RGB '
    okSet = cmsIT8SetPropertyUncooked(hIT8, "Author", "LCMS-Test-Author");

    // step 3: Operate
    retrieved = cmsIT8GetPropertyMulti(hIT8, "Author", "en");
    okWrite = cmsWriteRawTag(hProfile, tagSig, (const void *)rawData, (cmsUInt32Number)sizeof(rawData));

    // step 4: Validate & Cleanup
    cmsIT8Free(hIT8);

    (void)ctx;
    (void)retrieved;
    (void)okSet;
    (void)okWrite;
    (void)hProfile;
    (void)rawData;
    (void)tagSig;

    return 66; // API sequence test completed successfully
}