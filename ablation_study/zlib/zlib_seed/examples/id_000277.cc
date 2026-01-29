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
//<ID> 277
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compute Adler-32, and compress into a buffer
    const char src_c[] = "zlib API sequence payload: prepare -> compress -> inflate -> deflate cleanup";
    const Bytef *source = (const Bytef *)src_c;
    z_size_t src_zlen = (z_size_t)(sizeof(src_c) - 1);
    uLong sourceLen = (uLong)src_zlen;
    uLong adler_initial = adler32_z(0UL, source, src_zlen);
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream, inflate the compressed data, compute Adler on output, and cleanup inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLongf decompAlloc = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    int rc_inflate = inflate(&istrm, 0);
    z_size_t decompLen = (z_size_t)istrm.total_out;
    uLong adler_after_inflate = adler32_z(0UL, decompBuf, decompLen);
    int rc_inf_end = inflateEnd(&istrm);

    // step 3: Initialize a deflate stream (to demonstrate lifecycle), then end it
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = decompBuf;
    dstrm.avail_in = (uInt)decompLen;
    uLong bound2 = compressBound((uLong)decompLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)bound2);
    memset(outBuf, 0, (size_t)bound2);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)bound2;
    int rc_def_end = deflateEnd(&dstrm);

    // step 4: Cleanup memory and finalize
    free(compBuf);
    free(decompBuf);
    free(outBuf);
    (void)adler_initial;
    (void)adler_after_inflate;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_def_init;
    (void)rc_def_end;
    (void)version;
    (void)sourceLen;
    (void)decompAlloc;
    (void)bound;
    (void)bound2;
    // API sequence test completed successfully
    return 66;
}