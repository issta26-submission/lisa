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
//<ID> 177
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    cmsHPROFILE hProfile = cmsCreateNULLProfile();
    const char * key = "SampleKey";
    const char * subkey = "SampleSub";
    const char * value = "Example IT8 property value";

    // step 2: Configure (populate IT8 and profile header)
    cmsBool setok = cmsIT8SetPropertyMulti(hIT8, key, subkey, value);
    cmsSetProfileVersion(hProfile, 4.300000); // set profile version 4.3
    cmsSetHeaderModel(hProfile, (cmsUInt32Number)0x58595A20); // 'XYZ ' model

    // step 3: Operate (retrieve property and write it as a raw tag into the profile)
    const char * prop = cmsIT8GetPropertyMulti(hIT8, key, subkey);
    cmsUInt32Number propSize = (cmsUInt32Number)(strlen(prop) + 1);
    cmsWriteRawTag(hProfile, (cmsTagSignature)0x64617461 /* 'data' */, (const void *)prop, propSize);

    // step 4: Validate & Cleanup
    cmsIT8Free(hIT8);

    (void)ctx;
    (void)hProfile;
    (void)setok;
    (void)propSize;
    (void)prop;
    (void)key;
    (void)subkey;
    (void)value;

    // API sequence test completed successfully
    return 66;
}