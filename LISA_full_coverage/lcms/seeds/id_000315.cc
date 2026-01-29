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
//<ID> 315
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Use NULL context for THR APIs
    cmsContext ctx = (cmsContext)NULL;

    // Small in-memory blob (we do not have an IT8 loader in the available API list)
    const char it8_mem[] = "SampleKey: SampleValue\n";
    cmsHANDLE hIT8 = (cmsHANDLE)NULL;
    const char * propVal = "X";

    // Create a NULL profile using the thread-aware variant (pass NULL context)
    cmsHPROFILE profile = cmsCreateNULLProfileTHR(ctx);

    // Query profile version (float64) and fold to 32-bit for later use
    cmsFloat64Number ver_f = cmsGetProfileVersion(profile);
    cmsUInt32Number ver32 = (cmsUInt32Number) ver_f;

    // Determine number of channels from a (placeholder) colorspace signature (0)
    cmsColorSpaceSignature cs_placeholder = (cmsColorSpaceSignature)0;
    cmsUInt32Number channels = cmsChannelsOf(cs_placeholder);

    // Allocate a small buffer based on channel count and initialize it
    unsigned char * channelBuf = (unsigned char *) malloc((size_t)channels + 1);
    memset(channelBuf, (int)(ver32 & 0xFF), (size_t)channels + 1);
    channelBuf[0] = (unsigned char) propVal[0];

    // Ask how many bytes are needed to save the profile to memory, then save it
    cmsUInt32Number bytesNeeded = (cmsUInt32Number)0;
    cmsSaveProfileToMem(profile, (void *)NULL, &bytesNeeded);
    unsigned char * profMem = (unsigned char *) malloc((size_t)bytesNeeded + 1);
    memset(profMem, 0, (size_t)bytesNeeded + 1);
    cmsSaveProfileToMem(profile, (void *)profMem, &bytesNeeded);

    // Retrieve alarm codes (thread-aware and global variant)
    cmsUInt16Number alarms_thr[16];
    cmsGetAlarmCodesTHR(ctx, alarms_thr);
    cmsUInt16Number alarms[16];
    cmsGetAlarmCodes(alarms);

    // Cleanup allocated memory and unused handles
    free(profMem);
    free(channelBuf);
    cmsIT8Free(hIT8);

    return 66;
}