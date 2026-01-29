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
//<ID> 339
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(ctx);
    cmsUInt16Number alarms[16];
    memset(alarms, 0, sizeof(alarms));

    // step 2: Setup
    cmsColorSpaceSignature cs = cmsGetColorSpace(nullProfile);
    cmsUInt32Number channels = cmsChannelsOfColorSpace(cs);
    cmsUInt32Number nBytes = channels * (cmsUInt32Number)sizeof(cmsUInt16Number);
    cmsBool lIsFloat = 0;

    // step 3: Operate & Validate
    cmsGetAlarmCodes(alarms);
    cmsUInt32Number formatter = cmsFormatterForPCSOfProfile(nullProfile, nBytes, lIsFloat);
    cmsUInt32Number validation = formatter + (cmsUInt32Number)alarms[0];

    // step 4: Cleanup
    cmsCloseProfile(nullProfile);
    (void)validation;
    // API sequence test completed successfully
    return 66;
}