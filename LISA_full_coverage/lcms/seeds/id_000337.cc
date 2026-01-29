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
//<ID> 337
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

    // step 2: Setup / Configure
    cmsColorSpaceSignature cs = cmsGetColorSpace(nullProfile);
    cmsUInt32Number channels = (cmsUInt32Number) cmsChannelsOfColorSpace(cs);
    cmsUInt32Number nBytes = channels * (cmsUInt32Number) sizeof(cmsUInt16Number);
    cmsSetPCS(nullProfile, cs);

    // step 3: Operate / Validate
    cmsUInt32Number formatter = cmsFormatterForPCSOfProfile(nullProfile, nBytes, (cmsBool)0);
    cmsGetAlarmCodes(alarms);
    (void)formatter;
    (void)channels;
    (void)alarms[0];

    // step 4: Cleanup
    cmsCloseProfile(nullProfile);
    // API sequence test completed successfully
    return 66;
}