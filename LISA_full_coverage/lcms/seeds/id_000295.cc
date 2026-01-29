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
//<ID> 295
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
    cmsUInt32Number bytesNeeded = (cmsUInt32Number)0;
    cmsBool savedQuery = cmsSaveProfileToMem(inProfile, NULL, &bytesNeeded);
    unsigned char * memBuffer = (unsigned char *)malloc((size_t)bytesNeeded);
    memset(memBuffer, 0, (size_t)bytesNeeded);
    cmsBool savedToMem = cmsSaveProfileToMem(inProfile, memBuffer, &bytesNeeded);

    // step 2: Configure
    const char * fileName = "temp_profile.icc";
    cmsBool savedToFile = cmsSaveProfileToFile(inProfile, fileName);
    cmsUInt32Number inFormat = (cmsUInt32Number)0;
    cmsUInt32Number outFormat = (cmsUInt32Number)0;
    cmsUInt32Number intent = (cmsUInt32Number)0;
    cmsUInt32Number flags = (cmsUInt32Number)0;

    // step 3: Operate
    cmsHTRANSFORM transform = cmsCreateTransform(inProfile, inFormat, outProfile, outFormat, intent, flags);
    cmsUInt32Number transformInFmt = cmsGetTransformInputFormat(transform);
    (void)transformInFmt;
    cmsPipeline * pipeline = (cmsPipeline *)NULL;
    cmsPipelineFree(pipeline);

    // step 4: Validate & Cleanup
    cmsCloseProfile(inProfile);
    cmsCloseProfile(outProfile);
    free(memBuffer);
    (void)savedQuery;
    (void)savedToMem;
    (void)savedToFile;
    /* API sequence test completed successfully */
    return 66;
}