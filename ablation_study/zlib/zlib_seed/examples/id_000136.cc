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
//<ID> 136
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
    const char src_c[] = "zlib API sequence payload: compress2 -> store gz -> checksum -> uncompress -> inflateSyncPoint";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *mode_write = "wb";

    // step 2: Setup & Configure (compress)
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    uLong compressed_size = (uLong)compLen;

    // step 3: Operate (write compressed data to gz, compute checksum, uncompress, prepare inflate stream)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", mode_write);
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compressed_size);
    int rc_gz_close = gzclose(gz);
    uLong checksum = adler32_z(0UL, source, (z_size_t)sourceLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    uLongf outLen = (uLongf)sourceLen;
    int rc_un = uncompress(outBuf, &outLen, compBuf, compressed_size);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compressed_size;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)sourceLen;
    int rc_sync = inflateSyncPoint(&istrm);

    // step 4: Validate & Cleanup
    free(compBuf);
    free(outBuf);
    (void)rc_comp; (void)rc_gz_write; (void)rc_gz_close; (void)checksum; (void)rc_un; (void)rc_sync; (void)istrm;
    // API sequence test completed successfully
    return 66;
}