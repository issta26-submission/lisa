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
//<ID> 135
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char src_c[] = "zlib API sequence payload: compress2 -> gzopen -> inflateSyncPoint -> uncompress -> checksum";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLongf compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    uLongf compLen = compBound;
    uLongf uncompLen = (uLongf)sourceLen;

    // step 2: Setup (compress data)
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Operate (write compressed data to gz file, prepare inflate stream and check sync point, then uncompress and checksum)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)sourceLen;
    int rc_sync = inflateSyncPoint(&istrm);
    int rc_uncomp = uncompress(outBuf, &uncompLen, compBuf, (uLong)compLen);
    uLong checksum = adler32_z(0UL, source, (z_size_t)sourceLen);

    // step 4: Validate & Cleanup
    int rc_inf_end = inflateEnd(&istrm);
    int rc_gz_close = gzclose(gz);
    free(compBuf);
    free(outBuf);
    (void)rc_comp; (void)rc_gz_write; (void)rc_inf_init; (void)rc_sync; (void)rc_uncomp;
    (void)checksum; (void)rc_inf_end; (void)rc_gz_close;
    // API sequence test completed successfully
    return 66;
}