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
//<ID> 169
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers and sources
    const char src_c[] = "zlib API sequence payload: compress -> uncompress -> crc32_z -> gzputs -> inflateBackInit_";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();

    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Configure (compress and initialize inflateBack)
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    z_stream backStrm;
    memset(&backStrm, 0, sizeof(z_stream));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int rc_infback_init = inflateBackInit_(&backStrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate & Validate (uncompress, compute CRC, write via gzputs)
    uLongf decompLen = (uLongf)(sourceLen + 16);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompLen);
    memset(decompBuf, 0, (size_t)decompLen);
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, (uLong)compLen);

    uLong crc = crc32_z(0UL, (const Bytef *)decompBuf, (z_size_t)decompLen);

    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_puts = gzputs(gz, "Decompressed payload written\n");
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup
    int rc_infback_end = inflateBackEnd(&backStrm);
    free(compBuf);
    free(decompBuf);

    (void)rc_comp;
    (void)rc_infback_init;
    (void)rc_uncomp;
    (void)crc;
    (void)rc_gz_puts;
    (void)rc_gz_close;
    (void)rc_infback_end;

    // API sequence test completed successfully
    return 66;
}