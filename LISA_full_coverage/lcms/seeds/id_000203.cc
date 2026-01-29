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
//<ID> 203
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsUInt16Number alarmCodes[16] = {0};
    alarmCodes[0] = 0x10;
    alarmCodes[1] = 0x20;
    cmsSetAlarmCodesTHR((cmsContext)NULL, alarmCodes);
    cmsHPROFILE hProfile = cmsCreateNULLProfile();

    // step 2: Configure (derive a value from profile version and store it)
    cmsFloat64Number version = cmsGetProfileVersion(hProfile);
    cmsUInt32Number manufacturer = (cmsUInt32Number)(version * 1000.0);
    cmsSetHeaderManufacturer(hProfile, manufacturer);

    // step 3: Operate (read back stored value)
    cmsUInt32Number readManufacturer = cmsGetHeaderManufacturer(hProfile);

    // step 4: Validate & Cleanup
    cmsCloseProfile(hProfile);
    (void)alarmCodes;
    (void)version;
    (void)manufacturer;
    (void)readManufacturer;

    // API sequence test completed successfully
    return 66;
}