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
//<ID> 203
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize sources
    const char src_c[] = "zlib API sequence payload: compress -> uncompress -> gzwrite -> gzoffset64 -> crc32_z";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);

    // step 2: Compress the source to obtain a compressed buffer
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 3: Uncompress back into a destination buffer
    uLongf decompLen = (uLongf)sourceLen;
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompLen);
    memset(decompBuf, 0, (size_t)decompLen);
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, (uLong)compLen);

    // step 4: Write decompressed data to a gz file, query offset, compute CRC, and cleanup
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, (voidpc)decompBuf, (unsigned int)decompLen);
    off64_t offset = gzoffset64(gz);
    uLong crc = crc32_z(0UL, decompBuf, (z_size_t)decompLen);
    int rc_gz_close = gzclose(gz);

    free(compBuf);
    free(decompBuf);

    (void)rc_comp;
    (void)rc_uncomp;
    (void)rc_gz_write;
    (void)offset;
    (void)crc;
    (void)rc_gz_close;

    // API sequence test completed successfully
    return 66;
}