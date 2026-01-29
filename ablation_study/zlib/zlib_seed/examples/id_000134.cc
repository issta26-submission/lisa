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
//<ID> 134
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "zlib API sequence payload: compress2 -> gzopen/gzwrite -> uncompress -> inflateSyncPoint";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong checksum = adler32_z(0UL, source, (z_size_t)sourceLen);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *outBuf1 = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf1, 0, (size_t)sourceLen);
    Bytef *outBuf2 = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf2, 0, (size_t)sourceLen);
    const char *version = zlibVersion();

    // step 2: Setup & Compress
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Persist compressed data via gzFile
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);
    int rc_gz_close = gzclose(gz);

    // step 4: Decompress & inflate stream operations, Validate & Cleanup
    uLongf outLen1 = (uLongf)sourceLen;
    int rc_uncomp = uncompress(outBuf1, &outLen1, compBuf, (uLong)compLen);
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf2;
    istrm.avail_out = (uInt)sourceLen;
    int rc_inflate = inflate(&istrm, 0);
    int rc_sync = inflateSyncPoint(&istrm);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf1);
    free(outBuf2);
    (void)checksum; (void)rc_comp; (void)rc_gz_write; (void)rc_gz_close;
    (void)rc_uncomp; (void)rc_inf_init; (void)rc_inflate; (void)rc_sync; (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}