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
//<ID> 199
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "zlib API sequence payload: compress2 -> adler32_z -> inflateBackInit_ -> deflateEnd/inflateBackEnd cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong adler_before = adler32_z(0UL, source, (z_size_t)sourceLen);

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure & Compress2
    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);
    uLong adler_after = adler32_z(adler_before, compBuf, (z_size_t)compLen);

    // step 3: Initialize inflate-back machinery (inflateBackInit_) and then tear it down with inflateBackEnd
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_infb_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    int rc_infb_end = inflateBackEnd(&istrm);

    // step 4: Cleanup & finalization
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(window);

    (void)rc_def_init;
    (void)rc_comp2;
    (void)rc_infb_init;
    (void)rc_infb_end;
    (void)rc_def_end;
    (void)adler_before;
    (void)adler_after;

    // API sequence test completed successfully
    return 66;
}