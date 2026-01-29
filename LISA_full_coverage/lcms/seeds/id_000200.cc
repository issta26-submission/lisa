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
//<ID> 200
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Forward declarations for LCMS types and functions used.
   In the real test environment the proper lcms header will provide these.
   Declarations are kept minimal to allow this function to compile when
   linked against the actual LCMS library. */
typedef void * cmsHPROFILE;
typedef struct _cmsContext_struct * cmsContext;
typedef struct _cms_io_handler cmsIOHANDLER;
typedef unsigned short cmsUInt16Number;
typedef unsigned int cmsUInt32Number;
typedef double cmsFloat64Number;
typedef int cmsBool;









int test_lcms_api_sequence() {
    /* step 1: Declarations & Initialization */
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsUInt16Number alarmCodes[16];
    memset(alarmCodes, 0, sizeof(alarmCodes));
    alarmCodes[0] = (cmsUInt16Number)0x0001;
    alarmCodes[15] = (cmsUInt16Number)0x00FF;
    FILE * stream = tmpfile();

    unsigned char membuf[512];
    memset(membuf, 0, sizeof(membuf));
    cmsIOHANDLER * io = cmsOpenIOhandlerFromMem(ctx, membuf, (cmsUInt32Number)sizeof(membuf), "w+");
    cmsHPROFILE hProfile = cmsCreateXYZProfileTHR(ctx);

    /* step 2: Configure */
    cmsSetProfileVersion(hProfile, (cmsFloat64Number)4.3);

    /* step 3: Operate */
    cmsUInt32Number ver = cmsGetEncodedICCversion(hProfile);
    cmsUInt32Number written = cmsSaveProfileToIOhandler(hProfile, io);

    /* step 4: Validate & Cleanup */
    cmsDeleteContext(ctx);
    fclose(stream);

    (void)ver;
    (void)written;
    (void)alarmCodes;
    (void)io;
    (void)hProfile;
    (void)stream;
    (void)ctx;

    /* API sequence test completed successfully */
    return 66;
}