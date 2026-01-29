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
//<ID> 279
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char src_c[] = "zlib API sequence payload: compressBound -> compress2 -> inflate -> checksum";
    const Bytef *source = (const Bytef *)src_c;
    z_size_t src_zlen = (z_size_t)(sizeof(src_c) - 1);
    uLong sourceLen = (uLong)src_zlen;
    uLong adler_src = adler32_z(0UL, source, (z_size_t)src_zlen);
    const char *version = zlibVersion();

    // step 2: Compute compression bound and compress source into buffer
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    uLong adler_comp = adler32_z(0UL, compBuf, (z_size_t)compLen);

    // step 3: Initialize deflate and inflate streams and perform inflate of compressed data
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    uLongf decompAlloc = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    int rc_inflate = inflate(&istrm, 0);
    uLong adler_decomp = adler32_z(0UL, decompBuf, (z_size_t)src_zlen);

    // step 4: Cleanup streams and memory
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(decompBuf);

    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_def_end;
    (void)rc_inf_end;
    (void)adler_src;
    (void)adler_comp;
    (void)adler_decomp;
    (void)version;
    (void)compBound;
    (void)compLen;
    (void)decompAlloc;
    // API sequence test completed successfully
    return 66;
}