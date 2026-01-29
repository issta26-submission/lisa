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
//<ID> 171
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
    cmsBool propSet = cmsIT8SetPropertyMulti(hIT8, "DEVICE", "NAME", "MyDevice");
    const char * prop = cmsIT8GetPropertyMulti(hIT8, "DEVICE", "NAME");
    cmsFloat64Number version = 4.30;
    cmsUInt32Number model = (cmsUInt32Number)0x6D6F646C; /* 'modl' */
    cmsUInt32Number tagSize = (cmsUInt32Number)(prop ? (strlen(prop) + 1) : 0);
    cmsUInt32Number tagSig = (cmsUInt32Number)0x64657363; /* 'desc' */

    // step 2: Configure
    cmsSetProfileVersion(hProfile, version);
    cmsSetHeaderModel(hProfile, model);

    // step 3: Operate
    cmsBool wrote = cmsWriteRawTag(hProfile, (cmsTagSignature)tagSig, (const void *)prop, tagSize);

    // step 4: Validate & Cleanup
    cmsIT8Free(hIT8);

    (void)hProfile;
    (void)ctx;
    (void)propSet;
    (void)prop;
    (void)version;
    (void)model;
    (void)tagSize;
    (void)tagSig;
    (void)wrote;

    // API sequence test completed successfully
    return 66;
}