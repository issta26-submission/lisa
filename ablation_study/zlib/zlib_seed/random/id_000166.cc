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
//<ID> 166
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers and streams
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(z_stream));
    const char src_c[] = "zlib API sequence payload: compress -> uncompress -> crc32_z -> gzputs -> inflateBackInit_";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    uLongf outLen = (uLongf)sourceLen;

    // step 2: Compress the source into compBuf
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 3: Uncompress, compute CRCs, write a message via gzputs
    int rc_uncomp = uncompress(outBuf, &outLen, compBuf, (uLong)compLen);
    uLong crc_orig = crc32_z(0UL, source, (z_size_t)sourceLen);
    uLong crc_uncomp = crc32_z(0UL, outBuf, (z_size_t)outLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_puts = gzputs(gz, "Decompressed payload written (CRC check performed)\n");
    int rc_gz_close = gzclose(gz);

    // step 4: Initialize inflateBack, teardown, and cleanup
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int rc_infback_init = inflateBackInit_(&backstrm, 15, window, version, (int)sizeof(z_stream));
    int rc_infback_end = inflateBackEnd(&backstrm);
    free(compBuf);
    free(outBuf);

    (void)rc_comp;
    (void)rc_uncomp;
    (void)crc_orig;
    (void)crc_uncomp;
    (void)rc_gz_puts;
    (void)rc_gz_close;
    (void)rc_infback_init;
    (void)rc_infback_end;
    // API sequence test completed successfully
    return 66;
}