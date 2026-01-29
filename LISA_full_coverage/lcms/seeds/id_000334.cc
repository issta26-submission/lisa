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
//<ID> 334
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

    // step 2: Configure
    cmsColorSpaceSignature cs = cmsGetColorSpace(nullProfile);
    cmsInt32Number channels = cmsChannelsOfColorSpace(cs);
    cmsUInt32Number nBytes = (cmsUInt32Number)(channels * (int)sizeof(cmsUInt16Number));

    // step 3: Operate and Validate
    cmsUInt32Number fmtNative = cmsFormatterForPCSOfProfile(nullProfile, nBytes, (cmsBool)0);
    cmsUInt32Number fmtFloat  = cmsFormatterForPCSOfProfile(nullProfile, nBytes, (cmsBool)1);
    cmsUInt16Number alarms[16];
    memset(alarms, 0, sizeof(alarms));
    cmsGetAlarmCodes(alarms);
    cmsUInt16Number validation = (cmsUInt16Number)(alarms[0] ^ alarms[1]);
    (void)fmtNative; (void)fmtFloat; (void)validation;

    // step 4: Cleanup
    memset(alarms, 0, sizeof(alarms));

    return 66;
    // API sequence test completed successfully
}