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
//<ID> 172
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE hProfile = cmsCreateNULLProfile();
    cmsSetProfileVersion(hProfile, 4.50);
    cmsUInt32Number model = (cmsUInt32Number)0x52474220; // 'RGB '
    cmsSetHeaderModel(hProfile, model);

    // step 2: Setup
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    cmsIT8SetPropertyStr(hIT8, "Author", "LibFuzz");
    const char * author = cmsIT8GetPropertyMulti(hIT8, "Author", "");
    unsigned char raw[16];
    memset(raw, 0, sizeof(raw));
    raw[0] = (author && author[0]) ? (unsigned char)author[0] : 0;
    raw[1] = (author && author[1]) ? (unsigned char)author[1] : 0;
    raw[2] = (author && author[2]) ? (unsigned char)author[2] : 0;
    raw[3] = (author && author[3]) ? (unsigned char)author[3] : 0;

    // step 3: Operate
    cmsTagSignature tagSig = (cmsTagSignature)0x64657363; // 'desc'
    cmsBool writeOk = cmsWriteRawTag(hProfile, tagSig, raw, (cmsUInt32Number)sizeof(raw));
    cmsBool saveOk = cmsSaveProfileToFile(hProfile, "fuzz_profile.icc");

    // step 4: Validate & Cleanup
    cmsIT8Free(hIT8);
    (void)ctx;
    (void)hProfile;
    (void)model;
    (void)author;
    (void)raw;
    (void)tagSig;
    (void)writeOk;
    (void)saveOk;

    // API sequence test completed successfully
    return 66;
}