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
//<ID> 165
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers and source
    const char src_c[] = "zlib API sequence payload: generate compressed data, uncompress, write gz, init inflateBack";
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

    // step 2: Compress the source and then uncompress into outBuf
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    int rc_uncomp = uncompress(outBuf, &outLen, compBuf, compLen);

    // step 3: Write a small message to a gzip file using gzputs and compute crc32_z on original and uncompressed data
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_puts = gzputs(gz, "zlib test: wrote compressed payload\n");
    int rc_gz_close = gzclose(gz);
    uLong crc_src = crc32_z(0UL, source, (z_size_t)sourceLen);
    uLong crc_uncomp = crc32_z(0UL, outBuf, (z_size_t)outLen);

    // step 4: Initialize inflateBack, finalize it, and cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int rc_infback_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    int rc_infback_end = inflateBackEnd(&istrm);

    free(compBuf);
    free(outBuf);

    (void)rc_comp;
    (void)rc_uncomp;
    (void)rc_gz_puts;
    (void)rc_gz_close;
    (void)crc_src;
    (void)crc_uncomp;
    (void)rc_infback_init;
    (void)rc_infback_end;

    // API sequence test completed successfully
    return 66;
}