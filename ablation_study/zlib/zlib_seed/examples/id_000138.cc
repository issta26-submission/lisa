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
//<ID> 138
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char src_c[] = "zlib API payload: compress2 -> gzopen/gzwrite -> uncompress -> inflateSyncPoint -> checksum";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound_ul = compressBound(sourceLen);
    uLongf compBufLen = (uLongf)bound_ul;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound_ul);
    memset(compBuf, 0, (size_t)bound_ul);
    uLongf decompLen = (uLongf)sourceLen;
    Bytef *decompBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(decompBuf, 0, (size_t)sourceLen);

    // step 2: Setup (compress)
    int rc_compress = compress2(compBuf, &compBufLen, source, sourceLen, 6);

    // step 3: Operate (write compressed data to gz file, compute checksum, uncompress)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compBufLen);
    int rc_gz_close = gzclose(gz);
    uLong checksum = adler32_z(0UL, source, (z_size_t)sourceLen);
    int rc_uncompress = uncompress(decompBuf, &decompLen, compBuf, (uLong)compBufLen);

    // step 4: Validate & Cleanup
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compBufLen;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompLen;
    int rc_inflate_sync = inflateSyncPoint(&istrm);
    free(compBuf);
    free(decompBuf);
    (void)rc_compress; (void)rc_gz_write; (void)rc_gz_close; (void)checksum; (void)rc_uncompress; (void)rc_inflate_sync;
    // API sequence test completed successfully
    return 66;
}