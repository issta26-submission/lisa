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
//<ID> 137
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
    const char src_c[] = "zlib API sequence payload: compress2 -> gzopen/gzwrite -> uncompress -> inflateSyncPoint";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLenF = (uLongf)bound;
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    uLongf outLenF = (uLongf)sourceLen;
    uLong checksum = adler32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Setup (compress and write compressed data to a .gz file)
    int rc_comp = compress2(compBuf, &compLenF, source, sourceLen, 6);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLenF);
    int rc_gz_close = gzclose(gz);

    // step 3: Core operations (uncompress, initialize inflate stream, check sync point)
    uLong compLen = (uLong)compLenF;
    int rc_un = uncompress(outBuf, &outLenF, compBuf, compLen);
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outLenF;
    int rc_sync = inflateSyncPoint(&istrm);
    int rc_inf_end = inflateEnd(&istrm);

    // step 4: Cleanup
    free(compBuf);
    free(outBuf);
    (void)checksum; (void)rc_comp; (void)rc_gz_write; (void)rc_gz_close;
    (void)rc_un; (void)rc_inf_init; (void)rc_sync; (void)rc_inf_end;

    // API sequence test completed successfully
    return 66;
}