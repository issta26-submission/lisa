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
//<ID> 333
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
    cmsHPROFILE xyzProfile = cmsCreateXYZProfileTHR(ctx);
    cmsColorSpaceSignature csNull = cmsGetColorSpace(nullProfile);
    cmsColorSpaceSignature csXyz = cmsGetColorSpace(xyzProfile);
    cmsUInt16Number alarms[16];
    memset(alarms, 0, sizeof(alarms));

    // step 2: Setup / Configure
    cmsUInt32Number formatterNull = cmsFormatterForPCSOfProfile(nullProfile, (cmsUInt32Number)8, (cmsBool)0);
    cmsUInt32Number formatterXyz = cmsFormatterForPCSOfProfile(xyzProfile, (cmsUInt32Number)12, (cmsBool)1);
    cmsGetAlarmCodes(alarms);

    // step 3: Operate / Validate
    cmsUInt32Number combined = formatterNull + formatterXyz + (cmsUInt32Number)csNull + (cmsUInt32Number)csXyz + (cmsUInt32Number)alarms[0];
    (void) combined;

    // step 4: Cleanup
    memset(alarms, 0, sizeof(alarms));
    nullProfile = (cmsHPROFILE)NULL;
    xyzProfile = (cmsHPROFILE)NULL;
    ctx = (cmsContext)NULL;
    // API sequence test completed successfully
    return 66;
}