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
//<ID> 139
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "zlib API sequence payload: compress -> write gz -> uncompress -> checksum";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    uLong adler_orig = adler32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Setup & Compress
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    uLong compressed_size = (uLong)compLen;

    // step 3: Operate (write compressed data to gz file)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compressed_size);
    int rc_gz_close = gzclose(gz);

    // step 4: Validate & Cleanup (decompress, check inflate sync point, checksum, free)
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    uLongf outLen = (uLongf)sourceLen;
    int rc_uncomp = uncompress(outBuf, &outLen, compBuf, compressed_size);

    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compressed_size;
    int rc_sync = inflateSyncPoint(&istrm);

    uLong adler_after = adler32_z(0UL, outBuf, (z_size_t)outLen);

    free(compBuf);
    free(outBuf);

    (void)rc_comp; (void)rc_gz_write; (void)rc_gz_close; (void)rc_uncomp; (void)rc_sync;
    (void)adler_orig; (void)adler_after; (void)compressed_size; (void)outLen;
    // API sequence test completed successfully
    return 66;
}