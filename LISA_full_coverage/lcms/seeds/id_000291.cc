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
//<ID> 291
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsHPROFILE inProfile = cmsCreateXYZProfile();
    cmsHPROFILE outProfile = cmsCreateXYZProfile();
    cmsUInt32Number inFormat = (cmsUInt32Number)0;
    cmsUInt32Number outFormat = (cmsUInt32Number)0;
    cmsHTRANSFORM transform = cmsCreateTransform(inProfile, inFormat, outProfile, outFormat, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 2: Configure
    cmsUInt32Number memSize = (cmsUInt32Number)1024;
    unsigned char memBuf[1024];
    memset(memBuf, 0, sizeof(memBuf));
    cmsSaveProfileToMem(inProfile, memBuf, &memSize);
    cmsSaveProfileToFile(outProfile, "out_profile.icc");

    // step 3: Operate
    unsigned char input[3];
    unsigned char output[3];
    memset(input, 0x7F, sizeof(input));
    memset(output, 0, sizeof(output));
    input[0] = (unsigned char)255u;
    input[1] = (unsigned char)128u;
    input[2] = (unsigned char)64u;
    cmsDoTransform(transform, (const void *)input, (void *)output, (cmsUInt32Number)1);
    cmsHPROFILE devLink = cmsTransform2DeviceLink(transform, (cmsFloat64Number)4.3, (cmsUInt32Number)0);
    cmsUInt32Number memSize2 = (cmsUInt32Number)2048;
    unsigned char memBuf2[2048];
    memset(memBuf2, 0, sizeof(memBuf2));
    cmsSaveProfileToMem(devLink, memBuf2, &memSize2);

    // step 4: Validate & Cleanup
    cmsPipeline * dup = cmsPipelineDup((const cmsPipeline *)NULL);
    cmsPipelineFree(dup);
    cmsCloseProfile(inProfile);
    cmsCloseProfile(outProfile);
    cmsCloseProfile(devLink);
    /* API sequence test completed successfully */
    return 66;
}