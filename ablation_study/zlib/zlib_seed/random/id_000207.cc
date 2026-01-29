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
//<ID> 207
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize sources
    const char src_c[] = "zlib API sequence payload: compress2 -> uncompress -> crc32_z -> gzwrite/gzoffset64";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLongf compBufAlloc = (uLongf)compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufAlloc);
    memset(compBuf, 0, (size_t)compBufAlloc);
    uLongf compLen = compBufAlloc;

    // step 2: Compress source into compBuf
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Uncompress, validate with crc32_z, write out with gzwrite and obtain offset via gzoffset64
    uLongf decompBufAlloc = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompBufAlloc);
    memset(decompBuf, 0, (size_t)decompBufAlloc);
    uLongf decompLen = decompBufAlloc;
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, (uLong)compLen);
    uLong crc_after = crc32_z(0UL, decompBuf, (z_size_t)decompLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, (voidpc)decompBuf, (unsigned int)decompLen);
    off64_t offset_after_write = gzoffset64(gz);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup
    free(compBuf);
    free(decompBuf);

    (void)rc_comp2;
    (void)rc_uncomp;
    (void)crc_after;
    (void)rc_gz_write;
    (void)offset_after_write;
    (void)rc_gz_close;

    // API sequence test completed successfully
    return 66;
}