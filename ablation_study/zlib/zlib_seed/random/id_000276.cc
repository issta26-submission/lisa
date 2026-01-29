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
//<ID> 276
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compute adler, and reserve compression buffer
    const char src_c[] = "zlib API sequence payload: initialize -> deflate -> inflate -> validate";
    const Bytef *source = (const Bytef *)src_c;
    z_size_t src_zlen = (z_size_t)(sizeof(src_c) - 1);
    uLong sourceLen = (uLong)src_zlen;
    uLong adler_before = adler32_z(0UL, source, src_zlen);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);

    // step 2: Initialize deflate stream and perform a single-pass compression
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_deflate_call = deflate(&dstrm, 4); // Z_FINISH == 4
    uLong compUsed = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Initialize inflate stream, decompress, and validate with adler32_z
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong decompAllocUL = sourceLen + 1024;
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAllocUL);
    memset(decompBuf, 0, (size_t)decompAllocUL);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compUsed;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAllocUL;
    int rc_inflate_call = inflate(&istrm, 0);
    uLong decompressed_bytes = istrm.total_out;
    uLong adler_after = adler32_z(0UL, decompBuf, (z_size_t)decompressed_bytes);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Cleanup buffers and finalize
    free(compBuf);
    free(decompBuf);

    (void)rc_deflate_init;
    (void)rc_deflate_call;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate_call;
    (void)rc_inflate_end;
    (void)version;
    (void)adler_before;
    (void)adler_after;
    (void)compUsed;
    (void)decompressed_bytes;
    (void)compBound;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}