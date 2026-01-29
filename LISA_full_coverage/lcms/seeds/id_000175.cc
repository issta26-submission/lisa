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
//<ID> 175
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE hProfile = cmsCreateNULLProfile();
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    cmsIOHANDLER * io = cmsOpenIOhandlerFromNULL(ctx);
    unsigned char rawData[16];
    memset(rawData, 0, sizeof(rawData));
    rawData[0] = 0x42;
    rawData[1] = 0x43;
    cmsFloat64Number version = 4.30;
    cmsUInt32Number headerModel = 0x52474220; // 'RGB '
    cmsUInt32Number readModel = 0;
    const char * it8Prop = NULL;
    cmsBool writeOk = 0;

    // step 2: Configure
    cmsSetProfileVersion(hProfile, version);
    cmsSetHeaderModel(hProfile, headerModel);
    it8Prop = cmsIT8GetPropertyMulti(hIT8, "SampleKey", "SampleSubKey");

    // step 3: Operate
    writeOk = cmsWriteRawTag(hProfile, (cmsTagSignature)0x74657374 /* 'test' */, rawData, (cmsUInt32Number)sizeof(rawData));
    readModel = cmsGetHeaderModel(hProfile);

    // step 4: Validate & Cleanup
    cmsIT8Free(hIT8);
    cmsCloseIOhandler(io);

    (void)ctx;
    (void)hProfile;
    (void)it8Prop;
    (void)writeOk;
    (void)readModel;
    (void)version;
    (void)headerModel;
    (void)rawData;

    // API sequence test completed successfully
    return 66;
}