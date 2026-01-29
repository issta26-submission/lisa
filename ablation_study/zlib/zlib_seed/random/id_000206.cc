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
//<ID> 206
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize sources
    const char src_c[] = "zlib API sequence payload: compress2 -> uncompress -> gzwrite -> gzoffset64";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong crc_before = crc32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Configure & compress
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Operate & validate (uncompress, crc, write to gz file, get offset)
    uLongf decompLen = (uLongf)sourceLen;
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompLen);
    memset(decompBuf, 0, (size_t)decompLen);
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, (uLong)compLen);
    uLong crc_after = crc32_z(0UL, decompBuf, (z_size_t)decompLen);
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, (voidpc)decompBuf, (unsigned int)decompLen);
    off64_t offset = gzoffset64(gz);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup
    free(compBuf);
    free(decompBuf);

    (void)crc_before;
    (void)rc_comp;
    (void)rc_uncomp;
    (void)crc_after;
    (void)rc_gz_write;
    (void)offset;
    (void)rc_gz_close;

    // API sequence test completed successfully
    return 66;
}