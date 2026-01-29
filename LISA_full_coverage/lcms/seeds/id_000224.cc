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
//<ID> 224
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
static int my_sampler(const cmsUInt16Number * in, cmsUInt16Number * out, void * cargo) {
    /* Simple sampler that does nothing meaningful but matches required signature.
       Avoid dereferencing input/output to be safe. */
    (void)in;
    (void)out;
    (void)cargo;
    return 1;
}

int test_lcms_api_sequence() {
    /* step 1: Declarations & Initialization */
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHANDLE it8 = cmsIT8Alloc(ctx);
    cmsHPROFILE hProfile = cmsCreateXYZProfile();
    cmsUInt32Number headerModel = (cmsUInt32Number)0xABCDEF01U;

    static const cmsUInt32Number clutPts[3] = { 2U, 2U, 2U };

    /* step 2: Configure */
    cmsSetHeaderModel(hProfile, headerModel);

    /* step 3: Operate */
    const char * manuf = cmsIT8GetProperty(it8, "Manufacturer");
    cmsBool sliced = cmsSliceSpace16(3U, clutPts, (cmsSAMPLER16) my_sampler, (void *)manuf);

    /* step 4: Validate & Cleanup */
    cmsIT8Free(it8);
    cmsCloseProfile(hProfile);

    (void)ctx;
    (void)headerModel;
    (void)manuf;
    (void)sliced;

    /* API sequence test completed successfully */
    return 66;
}