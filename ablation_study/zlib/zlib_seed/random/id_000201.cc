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
//<ID> 201
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize sources
    const char src_c[] = "zlib API sequence payload: crc32_z -> compress -> uncompress -> gzwrite -> gzoffset64";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong crc_before = crc32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Configure & compress
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 3: Operate & Validate (uncompress, crc, write to gz)
    uLongf decompAlloc = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    uLongf decompLen = decompAlloc;
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, (uLong)compLen);
    uLong crc_after = crc32_z(0UL, decompBuf, (z_size_t)decompLen);
    gzFile gz = gzopen64("test_zlib_api_sequence_out.gz", "wb");
    int rc_gz_write = gzwrite(gz, (voidpc)decompBuf, (unsigned int)decompLen);
    off64_t offset_after_write = gzoffset64(gz);

    // step 4: Cleanup
    int rc_gz_close = gzclose(gz);
    free(compBuf);
    free(decompBuf);

    (void)crc_before;
    (void)rc_comp;
    (void)rc_uncomp;
    (void)crc_after;
    (void)rc_gz_write;
    (void)offset_after_write;
    (void)rc_gz_close;

    // API sequence test completed successfully
    return 66;
}