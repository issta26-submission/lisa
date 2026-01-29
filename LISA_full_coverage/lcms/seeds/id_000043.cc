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
//<ID> 43
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);

    // Create an sRGB profile using the context
    cmsHPROFILE prof = cmsCreate_sRGBProfileTHR(ctx);

    // Query some ASCII profile info into a local buffer
    char infoBuf[128];
    cmsUInt32Number infoSize = cmsGetProfileInfoASCII(prof, (cmsInfoType)0, "en", "US", infoBuf, sizeof(infoBuf));

    // Get tag count for the profile
    cmsUInt32Number tagCount = cmsGetTagCount(prof);

    // Allocate a pipeline (3 in, 3 out)
    cmsPipeline * pipe = cmsPipelineAlloc(ctx, 3U, 3U);

    // Query pipeline output channels and pipeline's context id
    cmsUInt32Number outChannels = cmsPipelineOutputChannels(pipe);
    cmsContext pipeCtx = cmsGetPipelineContextID(pipe);

    // Modify some profile header fields and read them back
    cmsSetProfileVersion(prof, 4.20);
    cmsSetHeaderRenderingIntent(prof, 0U);
    cmsUInt32Number intent = cmsGetHeaderRenderingIntent(prof);

    // Close the profile
    cmsCloseProfile(prof);

    // Silence unused variable warnings for values intentionally read
    (void) ctx;
    (void) infoSize;
    (void) tagCount;
    (void) pipe;
    (void) outChannels;
    (void) pipeCtx;
    (void) intent;
    (void) infoBuf;

    return 66;
}