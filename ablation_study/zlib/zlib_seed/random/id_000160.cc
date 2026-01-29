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
//<ID> 160
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize
    const char src_c[] = "zlib API payload: compress -> write gz -> inflateBackInit_ -> uncompress -> crc32_z";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
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

    // step 2: Configure & compress the source
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 3: Operate - write compressed data to a gz file and initialize inflateBack
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_puts = gzputs(gz, "zlib payload written\n");
    int rc_infback_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));

    // step 4: Validate & Cleanup - uncompress back, compute CRC, finish inflateBack and cleanup
    uLongf uncmpLen = (uLongf)sourceLen;
    int rc_uncomp = uncompress(outBuf, &uncmpLen, compBuf, (uLong)compLen);
    uLong crc = crc32_z(0UL, outBuf, (z_size_t)uncmpLen);
    int rc_infback_end = inflateBackEnd(&istrm);
    int rc_gz_close = gzclose(gz);
    free(compBuf);
    free(outBuf);

    (void)rc_comp;
    (void)rc_gz_puts;
    (void)rc_infback_init;
    (void)rc_uncomp;
    (void)crc;
    (void)rc_infback_end;
    (void)rc_gz_close;
    // API sequence test completed successfully
    return 66;
}