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
//<ID> 209
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize source, compute original CRC, compress
    const char src_c[] = "zlib API sequence payload: crc32_z -> compress2 -> uncompress -> gzwrite -> gzoffset64";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong crc_orig = crc32_z(0UL, source, (z_size_t)sourceLen);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);
    uLong crc_comp = crc32_z(0UL, compBuf, (z_size_t)compLen);

    // step 2: Prepare destination buffer and uncompress
    uLongf decompLen = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompLen);
    memset(decompBuf, 0, (size_t)decompLen);
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, (uLong)compLen);

    // step 3: Write decompressed data to a gzip file and get offset via gzoffset64
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, (voidpc)decompBuf, (unsigned int)decompLen);
    off64_t gz_off = gzoffset64(gz);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup
    free(compBuf);
    free(decompBuf);
    (void)crc_orig;
    (void)crc_comp;
    (void)rc_comp2;
    (void)rc_uncomp;
    (void)rc_gz_write;
    (void)gz_off;
    (void)rc_gz_close;

    // API sequence test completed successfully
    return 66;
}