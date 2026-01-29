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
//<ID> 170
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
    const char * propValue = NULL;
    cmsBool wrote = 0;
    cmsUInt32Number writeSize = 0;

    // step 2: Configure
    cmsIT8SetPropertyMulti(hIT8, "Manufacturer", "Product", "TestProductModel");
    propValue = cmsIT8GetPropertyMulti(hIT8, "Manufacturer", "Product");
    cmsSetProfileVersion(hProfile, (cmsFloat64Number)4.321);
    cmsSetHeaderModel(hProfile, (cmsUInt32Number)0x52474220U); // 'RGB ' as fourcc

    // step 3: Operate (write a raw tag using the retrieved property)
    writeSize = (cmsUInt32Number)(propValue ? strlen(propValue) + 1U : 0U);
    wrote = cmsWriteRawTag(hProfile, (cmsTagSignature)0x64617461U, propValue, writeSize); // 'data'
    cmsIT8Free(hIT8);

    // step 4: Validate & Cleanup
    (void)wrote;
    (void)writeSize;
    (void)propValue;
    (void)ctx;
    (void)hProfile;

    // API sequence test completed successfully
    return 66;
}