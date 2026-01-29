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
//<ID> 130
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "zlib API sequence payload: compress2 -> uncompress -> inflateSyncPoint -> checksum -> gzopen";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong checksum = adler32_z(0UL, source, (z_size_t)sourceLen);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Setup (compress and open gz file)
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);

    // step 3: Core operations (prepare inflate stream and uncompress)
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    uLongf outLen = (uLongf)sourceLen;
    int rc_uncomp = uncompress(outBuf, &outLen, compBuf, (uLong)compLen);
    int rc_inflate_sync = inflateSyncPoint(&istrm);

    // step 4: Validate & Cleanup
    int rc_inf_end = inflateEnd(&istrm);
    int rc_gz_close = gzclose(gz);
    free(compBuf);
    free(outBuf);
    (void)checksum; (void)bound; (void)rc_comp; (void)rc_gz_write;
    (void)rc_inf_init; (void)rc_uncomp; (void)rc_inflate_sync;
    (void)rc_inf_end; (void)rc_gz_close;
    // API sequence test completed successfully
    return 66;
}