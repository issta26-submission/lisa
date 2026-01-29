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
//<ID> 51
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE hNullProfile = cmsCreateNULLProfileTHR(ctx);
    cmsUInt32Number renderIntent = 0;
    cmsHANDLE hIT8 = (cmsHANDLE) NULL;
    char ** propertyNames = NULL;
    char *** pPropertyNames = &propertyNames;
    cmsUInt32Number propCount = 0;
    cmsStage * stage = (cmsStage *) NULL;
    void * stage_data = (void *) NULL;

    // step 2: Setup
    renderIntent = cmsGetHeaderRenderingIntent(hNullProfile);
    hIT8 = cmsIT8Alloc(ctx);
    propCount = cmsIT8EnumProperties(hIT8, pPropertyNames);

    // step 3: Core operations
    cmsUInt16Number clutTable[2];
    clutTable[0] = (cmsUInt16Number) (renderIntent & 0xFFFF);
    clutTable[1] = (cmsUInt16Number) (propCount & 0xFFFF);
    stage = cmsStageAllocCLut16bit(ctx, 2U, 1U, 1U, clutTable);
    stage_data = cmsStageData(stage);

    // step 4: Cleanup
    cmsStageFree(stage);
    cmsCloseProfile(hNullProfile);

    (void) ctx;
    (void) propertyNames;
    (void) stage_data;
    (void) hIT8;
    (void) renderIntent;
    (void) propCount;

    // API sequence test completed successfully
    return 66;
}