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
//<ID> 272
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute Adler-32 checksum
    const char src_c[] = "zlib API sequence payload: deflate -> inflate -> checksum validation";
    const Bytef *source = (const Bytef *)src_c;
    z_size_t src_zlen = (z_size_t)(sizeof(src_c) - 1);
    uLong sourceLen = (uLong)src_zlen;
    uLong adler_src = adler32_z(0UL, source, src_zlen);

    // step 2: Setup compressors/inflaters and allocate buffers
    const char *version = zlibVersion();
    uLong compBoundVal = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBoundVal);
    memset(compBuf, 0, (size_t)compBoundVal);
    z_stream dstream;
    memset(&dstream, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstream, 6, version, (int)sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Operate: deflate the source into compBuf, finalize, then inflate back and compute checksum
    dstream.next_in = (Bytef *)source;
    dstream.avail_in = (uInt)sourceLen;
    dstream.next_out = compBuf;
    dstream.avail_out = (uInt)compBoundVal;
    int rc_def = deflate(&dstream, 4);
    int rc_def_end = deflateEnd(&dstream);
    uLong compUsed = dstream.total_out;

    uLong decompAlloc = sourceLen + 64;
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compUsed;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    int rc_inf = inflate(&istrm, 0);
    uLong decompUsed = istrm.total_out;
    uLong adler_decomp = adler32_z(0UL, decompBuf, (z_size_t)decompUsed);

    // step 4: Cleanup inflate stream and free resources
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(decompBuf);

    (void)adler_src;
    (void)adler_decomp;
    (void)rc_def_init;
    (void)rc_inf_init;
    (void)rc_def;
    (void)rc_def_end;
    (void)rc_inf;
    (void)rc_inf_end;
    (void)compBoundVal;
    (void)compUsed;
    (void)decompUsed;
    (void)version;
    // API sequence test completed successfully
    return 66;
}