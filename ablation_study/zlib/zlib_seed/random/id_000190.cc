#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 190
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "zlib API sequence payload: compress2 -> inflateBackInit_ -> inflateBackEnd -> adler32_z";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Configure & Compress
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);
    uLong src_adler = adler32_z(1UL, source, (z_size_t)sourceLen);

    // step 3: Operate & Validate
    z_stream bstrm;
    memset(&bstrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_infback_init = inflateBackInit_(&bstrm, 15, window, version, (int)sizeof(z_stream));
    bstrm.next_in = compBuf;
    bstrm.avail_in = (uInt)compLen;
    Bytef *outBuf = (Bytef *)malloc((size_t)(sourceLen + 64));
    memset(outBuf, 0, (size_t)(sourceLen + 64));
    bstrm.next_out = outBuf;
    bstrm.avail_out = (uInt)(sourceLen + 64);
    uLong comp_adler = adler32_z(0UL, compBuf, (z_size_t)compLen);
    int rc_inf_back_end = inflateBackEnd(&bstrm);

    // step 4: Cleanup
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(window);
    free(outBuf);

    (void)rc_def_init;
    (void)rc_comp2;
    (void)src_adler;
    (void)rc_infback_init;
    (void)comp_adler;
    (void)rc_inf_back_end;
    (void)rc_def_end;

    // API sequence test completed successfully
    return 66;
}