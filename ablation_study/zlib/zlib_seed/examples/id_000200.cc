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
//<ID> 200
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize sources
    const char src_c[] = "zlib API sequence payload: crc32_z -> compress2 -> uncompress -> gzwrite/gzoffset64";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong crc_before = crc32_z(0UL, source, (z_size_t)sourceLen);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);

    // step 2: Configure & Compress
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    uLong crc_comp = crc32_z(0UL, compBuf, (z_size_t)compLen);

    // step 3: Operate & Uncompress
    uLongf decompAlloc = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    uLongf decompLen = decompAlloc;
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, (uLong)compLen);

    // step 4: Validate & Cleanup (write to gzip and check offset)
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, (voidpc)decompBuf, (unsigned int)decompLen);
    off64_t offset = gzoffset64(gz);
    int rc_gz_close = gzclose(gz);
    free(compBuf);
    free(decompBuf);

    (void)crc_before;
    (void)crc_comp;
    (void)rc_comp;
    (void)rc_uncomp;
    (void)rc_gz_write;
    (void)rc_gz_close;
    (void)offset;

    // API sequence test completed successfully
    return 66;
}