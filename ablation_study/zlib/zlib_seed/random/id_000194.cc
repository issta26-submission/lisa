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
//<ID> 194
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "zlib API sequence payload: compress2 -> inflateBackInit_ -> inflateBackEnd -> deflateEnd";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong adler_initial = adler32_z(1UL, source, (z_size_t)sourceLen);

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure & Compress
    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);
    uLong adler_after_comp = adler32_z(adler_initial, compBuf, (z_size_t)compLen);

    // step 3: Initialize inflate-back then teardown (inflateBackEnd)
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32);
    memset(window, 0, 32);
    int rc_inf_back_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    Bytef *decompBuf = (Bytef *)malloc((size_t)(sourceLen + 64));
    memset(decompBuf, 0, (size_t)(sourceLen + 64));
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)(sourceLen + 64);
    int rc_inf_back_end = inflateBackEnd(&istrm);

    // step 4: Cleanup & finalization
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(window);
    free(decompBuf);

    (void)adler_initial;
    (void)adler_after_comp;
    (void)rc_def_init;
    (void)rc_comp2;
    (void)rc_inf_back_init;
    (void)rc_inf_back_end;
    (void)rc_def_end;

    // API sequence test completed successfully
    return 66;
}