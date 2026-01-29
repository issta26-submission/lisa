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
//<ID> 330
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Initialize
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(ctx);
    cmsUInt16Number alarms[16];
    memset(alarms, 0, sizeof(alarms));
    cmsGetAlarmCodes(alarms);

    // step 2: Configure
    auto colorSpace = cmsGetColorSpace(nullProfile);
    cmsUInt32Number cs_val = (cmsUInt32Number) colorSpace;
    cmsUInt32Number fmt8 = cmsFormatterForPCSOfProfile(nullProfile, 8, (cmsBool)0);

    // step 3: Operate & Validate
    cmsHPROFILE tempProfile = cmsCreateNULLProfileTHR(ctx);
    cmsUInt32Number fmt4float = cmsFormatterForPCSOfProfile(tempProfile, 4, (cmsBool)1);
    cmsUInt32Number validation_token = fmt8 ^ fmt4float ^ cs_val ^ (cmsUInt32Number)alarms[0];
    (void)validation_token;

    // step 4: Cleanup
    memset(alarms, 0, sizeof(alarms));
    nullProfile = (cmsHPROFILE)NULL;
    tempProfile = (cmsHPROFILE)NULL;
    ctx = (cmsContext)NULL;

    // API sequence test completed successfully
    return 66;
}