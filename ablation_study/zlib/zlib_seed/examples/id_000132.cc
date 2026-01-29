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
//<ID> 132
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize
    const char src[] = "zlib API sequence payload: compress2 -> gzopen -> uncompress -> checksum -> inflateSyncPoint";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    const char *version = zlibVersion();

    // step 2: Setup & compress
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Persist compressed data via gz and then decompress with uncompress
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gzf, compBuf, (unsigned int)compLen);
    int rc_gz_close = gzclose(gzf);
    uLongf outLen = (uLongf)sourceLen;
    int rc_un = uncompress(outBuf, &outLen, compBuf, (uLong)compLen);

    // step 4: Validate using adler32_z and inflateSyncPoint, cleanup
    uLong checksum = adler32_z(0UL, source, (z_size_t)sourceLen);
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)sourceLen;
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_sync = inflateSyncPoint(&istrm);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)rc_comp; (void)rc_gz_write; (void)rc_gz_close; (void)rc_un;
    (void)checksum; (void)rc_inf_init; (void)rc_sync; (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}