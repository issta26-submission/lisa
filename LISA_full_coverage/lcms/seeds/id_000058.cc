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
//<ID> 58
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Declarations and simple derived sizes
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    cmsContext ctx = cmsGetProfileContextID(hProfile);
    cmsUInt32Number inputCh = 3U;
    cmsUInt32Number outputCh = 3U;
    cmsUInt32Number nGrid = inputCh + outputCh;
    cmsUInt32Number cubeSize = nGrid * nGrid * nGrid;
    cmsUInt32Number tableLen = cubeSize * outputCh;

    // Allocate and initialize a small table used by the test
    cmsUInt16Number * table = (cmsUInt16Number *) malloc((size_t)(tableLen * sizeof(cmsUInt16Number)));
    memset(table, 0, (size_t)(tableLen * sizeof(cmsUInt16Number)));

    // Create an identity stage and obtain its internal data pointer
    cmsStage * stage = cmsStageAllocIdentity(ctx, inputCh);
    void * stageData = cmsStageData(stage);

    // Create an IT8 handle and enumerate properties
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    char ** propNames = NULL;
    cmsUInt32Number propCount = (cmsUInt32Number) cmsIT8EnumProperties(hIT8, &propNames);

    // Store the property count into the table to exercise memory access
    table[0] = (cmsUInt16Number) propCount;

    // Cleanup (library-specific frees for some handles are not exposed in the provided API list)
    free(table);
    cmsCloseProfile(hProfile);

    // Indicate success
    return 66;
}