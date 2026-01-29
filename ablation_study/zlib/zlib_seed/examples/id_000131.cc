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
//<ID> 131
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
    const char src_c[] = "zlib API sequence payload for compress/uncompress/inflateSyncPoint/checksum/gzopen";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    uLongf outLen = (uLongf)sourceLen;
    const char *version = zlibVersion();

    // step 2: Compress using compress2
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Decompress using uncompress
    int rc_uncomp = uncompress(outBuf, &outLen, compBuf, (uLong)compLen);

    // step 4: Inflate sync check, checksum, write gz file, and cleanup
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    int rc_sync = inflateSyncPoint(&istrm);
    uLong checksum = adler32_z(0UL, source, (z_size_t)sourceLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);
    int rc_gz_close = gzclose(gz);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)rc_comp; (void)rc_uncomp; (void)rc_inf_init; (void)rc_sync; (void)checksum;
    (void)rc_gz_write; (void)rc_gz_close; (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}