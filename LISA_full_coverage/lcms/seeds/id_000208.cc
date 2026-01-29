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
//<ID> 208
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsUInt16Number alarmCodes[16] = {0};
    alarmCodes[0] = (cmsUInt16Number)0x0100;
    alarmCodes[1] = (cmsUInt16Number)0x0200;
    cmsHPROFILE hProfile = (cmsHPROFILE)NULL;
    cmsFloat64Number tac = 0.0;
    cmsUInt16Number tacRounded = 0;
    cmsUInt32Number bytesNeeded = 0;
    void * mem = (void *)NULL;

    // step 2: Configure (initial alarm codes)
    cmsSetAlarmCodes(alarmCodes);

    // step 3: Create profile and operate
    hProfile = cmsCreateNULLProfileTHR(ctx);
    tac = cmsDetectTAC(hProfile);
    tacRounded = (cmsUInt16Number)(tac + 0.5);
    alarmCodes[2] = tacRounded;
    cmsSetAlarmCodes(alarmCodes);

    // step 4: "IO" via memory (reserve + save) — straight-line, no branches
    cmsSaveProfileToMem(hProfile, NULL, &bytesNeeded);
    mem = malloc((size_t)(bytesNeeded ? bytesNeeded : 1));
    cmsSaveProfileToMem(hProfile, mem, &bytesNeeded);
    free(mem);

    // step 5: Cleanup
    cmsCloseProfile(hProfile);

    (void)ctx;
    (void)tac;
    (void)tacRounded;
    (void)alarmCodes;
    (void)bytesNeeded;
    (void)mem;
    // API sequence test completed successfully
    return 66;
}