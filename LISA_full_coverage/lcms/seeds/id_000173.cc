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
//<ID> 173
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
    const char * propVal = NULL;
    cmsBool wrote = 0;
    cmsUInt32Number tagSig = (cmsUInt32Number)0;
    cmsUInt32Number dataSize = 0;

    // step 2: Configure (set an IT8 property and adjust profile header/version)
    cmsIT8SetPropertyUncooked(hIT8, "Manufacturer", "ACME_Inc");
    propVal = cmsIT8GetPropertyMulti(hIT8, "Manufacturer", NULL);
    cmsSetProfileVersion(hProfile, (cmsFloat64Number)4.30);
    cmsSetHeaderModel(hProfile, (cmsUInt32Number)0x00000000);

    // step 3: Operate (write the retrieved IT8 property as a raw tag into the profile and save)
    dataSize = (cmsUInt32Number)(propVal ? (strlen(propVal) + 1U) : 0U);
    cmsWriteTag(hProfile, (cmsTagSignature)tagSig, propVal);
    cmsSaveProfileToFile(hProfile, "test_profile.icc");

    // step 4: Validate & Cleanup
    cmsIT8Free(hIT8);

    (void)ctx;
    (void)hProfile;
    (void)propVal;
    (void)wrote;
    (void)tagSig;
    (void)dataSize;

    return 66;
}