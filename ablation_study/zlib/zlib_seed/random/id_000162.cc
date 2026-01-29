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
//<ID> 162
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers and source
    const char src_c[] = "zlib API sequence payload: compress -> uncompress -> crc -> inflateBackInit -> gzputs";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c)); // include terminating NUL so the uncompressed buffer is a C string
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));

    // step 2: Compress the source and then uncompress it
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    uLongf outLen = (uLongf)sourceLen;
    int rc_uncomp = uncompress(outBuf, &outLen, compBuf, (uLong)compLen);

    // step 3: Compute CRC of the uncompressed payload, initialize inflateBack, and write the decompressed C-string via gzputs
    uLong crc = crc32_z(0UL, outBuf, (z_size_t)(outLen - 1)); // exclude terminating NUL from CRC
    int rc_infback_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_puts = gzputs(gz, (const char *)outBuf);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup & finalize
    int rc_infback_end = inflateBackEnd(&istrm);
    free(compBuf);
    free(outBuf);

    (void)rc_comp;
    (void)rc_uncomp;
    (void)crc;
    (void)rc_infback_init;
    (void)rc_gz_puts;
    (void)rc_gz_close;
    (void)rc_infback_end;

    // API sequence test completed successfully
    return 66;
}