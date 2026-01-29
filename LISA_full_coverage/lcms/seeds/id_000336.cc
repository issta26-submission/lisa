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
//<ID> 336
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
    cmsUInt16Number alarms[16];
    memset(alarms, 0, sizeof(alarms));
    cmsGetAlarmCodes(alarms);
    cmsUInt32Number cs = (cmsUInt32Number) cmsGetColorSpace(nullProfile);

    // step 3: Operate & Validate
    cmsUInt32Number baseBytes = 8u;
    cmsUInt32Number bytes1 = baseBytes + (cs & 3u);
    cmsBool lIsFloat1 = (cmsBool)0;
    cmsUInt32Number fmt1 = cmsFormatterForPCSOfProfile(nullProfile, bytes1, lIsFloat1);
    cmsUInt32Number additive = (cmsUInt32Number) alarms[0];
    cmsUInt32Number bytes2 = fmt1 + additive + 1u;
    cmsBool lIsFloat2 = (cmsBool)(fmt1 & 1u);
    cmsUInt32Number fmt2 = cmsFormatterForPCSOfProfile(nullProfile, bytes2, lIsFloat2);
    (void) fmt1;
    (void) fmt2;

    // step 4: Cleanup
    cmsCloseProfile(nullProfile);
    // API sequence test completed successfully
    return 66;
}