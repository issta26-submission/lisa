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
//<ID> 205
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize sources
    const char src_c[] = "zlib API sequence payload: prepare -> compress2 -> crc32_z -> uncompress -> gzwrite -> gzoffset64";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();

    // step 2: Configure & compress
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Operate & Validate (crc, uncompress, write gz, get offset)
    uLong crc = crc32_z(0UL, compBuf, (z_size_t)compLen);
    uLongf decompCap = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompCap);
    memset(decompBuf, 0, (size_t)decompCap);
    int rc_uncomp = uncompress(decompBuf, &decompCap, compBuf, (uLong)compLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, (voidpc)decompBuf, (unsigned int)decompCap);
    off64_t offset = gzoffset64(gz);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup
    free(compBuf);
    free(decompBuf);
    (void)version;
    (void)rc_comp;
    (void)crc;
    (void)rc_uncomp;
    (void)rc_gz_write;
    (void)offset;
    (void)rc_gz_close;

    // API sequence test completed successfully
    return 66;
}