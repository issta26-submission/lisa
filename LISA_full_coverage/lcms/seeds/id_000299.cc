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
//<ID> 299
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = (cmsContext)NULL;
    cmsHPROFILE inProfile = cmsCreateNULLProfileTHR(ctx);
    cmsHPROFILE outProfile = cmsCreateNULLProfileTHR(ctx);
    cmsUInt32Number inFormat = (cmsUInt32Number)0;
    cmsUInt32Number outFormat = (cmsUInt32Number)0;
    cmsUInt32Number intent = (cmsUInt32Number)0;
    cmsUInt32Number flags = (cmsUInt32Number)0;
    cmsHTRANSFORM transform = cmsCreateTransform(inProfile, inFormat, outProfile, outFormat, intent, flags);

    // step 2: Configure
    cmsUInt32Number bytesNeeded = (cmsUInt32Number)0;
    cmsBool gotSize = cmsSaveProfileToMem(inProfile, (void*)NULL, &bytesNeeded);
    cmsUInt32Number allocSize = (bytesNeeded != (cmsUInt32Number)0) ? bytesNeeded : (cmsUInt32Number)1;
    unsigned char * memBuffer = (unsigned char *)malloc((size_t)allocSize);
    memset(memBuffer, 0, (size_t)allocSize);
    cmsBool savedToMem = cmsSaveProfileToMem(inProfile, (void *)memBuffer, &bytesNeeded);

    // step 3: Operate
    cmsBool savedToFile = cmsSaveProfileToFile(outProfile, "out_profile.icc");
    cmsPipeline * lut = (cmsPipeline *)NULL;
    cmsPipelineFree(lut);

    // step 4: Validate & Cleanup
    cmsCloseProfile(inProfile);
    cmsCloseProfile(outProfile);
    free(memBuffer);
    /* API sequence test completed successfully */
    return 66;
}