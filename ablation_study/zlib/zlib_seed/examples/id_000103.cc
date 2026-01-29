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
//<ID> 103
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream infstrm;
    z_stream copystrm;
    memset(&infstrm, 0, sizeof(z_stream));
    memset(&copystrm, 0, sizeof(z_stream));
    const char src_c[] = "zlib payload: compress -> inflateInit_ -> inflateCopy -> inflate";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    const char *version = zlibVersion();

    // step 2: Setup (compress source and initialize inflate)
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    int rc_inf_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)compLen;
    infstrm.next_out = outBuf;
    infstrm.avail_out = (uInt)sourceLen;

    // step 3: Operate (copy inflate state, run inflate on the copy, write compressed data to gz and put a char)
    int rc_inf_copy = inflateCopy(&copystrm, &infstrm);
    int rc_inf_copy_run = inflate(&copystrm, 0);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);
    int rc_gz_putc = gzputc(gz, 'Z');
    int rc_gz_close = gzclose(gz);

    // step 4: Validate & Cleanup
    (void)rc_comp; (void)rc_inf_init; (void)rc_inf_copy; (void)rc_inf_copy_run; (void)rc_gz_write; (void)rc_gz_putc; (void)rc_gz_close;
    inflateEnd(&infstrm);
    inflateEnd(&copystrm);
    free(compBuf);
    free(outBuf);
    // API sequence test completed successfully
    return 66;
}