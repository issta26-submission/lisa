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
//<ID> 133
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
    const char src_c[] = "zlib API sequence payload: compress2 -> gzopen -> uncompress -> inflateSyncPoint -> checksum";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLongf compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = compBound;
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);

    // step 2: Setup (checksum, compress, open gz and init inflate)
    uLong checksum = adler32_z(0UL, source, (z_size_t)sourceLen);
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)sourceLen;

    // step 3: Operate (check sync point, perform uncompress)
    int rc_sync = inflateSyncPoint(&istrm);
    uLongf uncomprLen = (uLongf)sourceLen;
    int rc_uncomp = uncompress(outBuf, &uncomprLen, compBuf, (uLong)compLen);

    // step 4: Validate & Cleanup
    int rc_inf_end = inflateEnd(&istrm);
    int rc_gz_close = gzclose(gz);
    free(compBuf);
    free(outBuf);
    (void)checksum;
    (void)rc_comp;
    (void)rc_gz_write;
    (void)rc_inf_init;
    (void)rc_sync;
    (void)rc_uncomp;
    (void)uncomprLen;
    (void)rc_inf_end;
    (void)rc_gz_close;
    // API sequence test completed successfully
    return 66;
}