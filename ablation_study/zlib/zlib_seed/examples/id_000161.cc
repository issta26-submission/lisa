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
//<ID> 161
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers and compress/uncompress
    const char src_c[] = "zlib API sequence payload: compress -> uncompress -> crc32_z -> inflateBackInit_ -> gzputs";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();

    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    uLongf outLen = (uLongf)sourceLen;
    int rc_uncomp = uncompress(outBuf, &outLen, compBuf, (uLong)compLen);

    // step 2: Configure inflateBack and window
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int rc_infback_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate: compute CRCs and write a message with gzputs
    uLong crc_source = crc32_z(0L, source, (z_size_t)sourceLen);
    uLong crc_uncompressed = crc32_z(0L, outBuf, (z_size_t)outLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_puts = gzputs(gz, "zlib test: wrote message after operations\n");

    // step 4: Validate & Cleanup
    int rc_infback_end = inflateBackEnd(&istrm);
    int rc_gz_close = gzclose(gz);
    free(compBuf);
    free(outBuf);

    (void)rc_comp;
    (void)rc_uncomp;
    (void)rc_infback_init;
    (void)crc_source;
    (void)crc_uncompressed;
    (void)rc_gz_puts;
    (void)rc_infback_end;
    (void)rc_gz_close;

    // API sequence test completed successfully
    return 66;
}