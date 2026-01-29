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
//<ID> 338
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations and context creation
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(ctx);
    cmsUInt16Number alarmCodes[16];
    memset(alarmCodes, 0, sizeof(alarmCodes));

    // step 2: Query alarm codes and profile color space
    cmsGetAlarmCodes(alarmCodes);
    cmsColorSpaceSignature colorSpace = cmsGetColorSpace(nullProfile);

    // step 3: Determine formatter parameters and invoke formatter
    cmsUInt32Number nBytes = (cmsUInt32Number)(4 + (((cmsUInt32Number)colorSpace & 0xFFU) != 0U) * 4U);
    cmsBool isFloat = 0;
    cmsUInt32Number formatter = cmsFormatterForPCSOfProfile(nullProfile, nBytes, isFloat);

    // step 4: Use results and cleanup
    cmsUInt32Number summary = (cmsUInt32Number)alarmCodes[0] + (cmsUInt32Number)alarmCodes[1] + formatter + (cmsUInt32Number)colorSpace;
    (void) summary;
    cmsCloseProfile(nullProfile);

    // API sequence test completed successfully
    return 66;
}