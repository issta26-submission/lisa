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
//<ID> 9
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = NULL;
    cmsHPROFILE profile = NULL;
    cmsUInt32Number supported = 4;
    cmsUInt32Number Codes[4];
    char *Descriptions[4];
    Codes[0] = Codes[1] = Codes[2] = Codes[3] = 0;
    Descriptions[0] = Descriptions[1] = Descriptions[2] = Descriptions[3] = NULL;
    cmsPipeline *pipeline = NULL;
    cmsStage *stage = NULL;
    cmsFloat64Number version = 0.0;

    profile = cmsCreate_sRGBProfileTHR(ctx);
    (void) cmsGetSupportedIntents(supported, Codes, Descriptions);
    version = (cmsFloat64Number) cmsGetHeaderFlags(profile);

    stage = cmsStageAllocIdentity(ctx, 3U);
    (void) cmsStageType(stage);
    (void) cmsStageInputChannels(stage);
    (void) cmsStageData(stage);
    cmsStage *dup = cmsStageDup(stage);
    (void) cmsStageNext(dup);

    (void) pipeline;
    (void) dup;
    (void) version;
    return 66;
}