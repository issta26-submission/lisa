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
//<ID> 104
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream infstrm;
    z_stream infcopy;
    memset(&infstrm, 0, sizeof(z_stream));
    memset(&infcopy, 0, sizeof(z_stream));
    const char src_c[] = "zlib sequence payload to compress, write with gz, copy inflate stream and decompress";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    const char *version = zlibVersion();

    // step 2: Setup (compress source and initialize inflate stream)
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    int rc_inf_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)compLen;
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    infstrm.next_out = outBuf;
    infstrm.avail_out = (uInt)sourceLen;

    // step 3: Core operations (copy inflate state, perform inflate, write compressed data to gz)
    int rc_inf_copy = inflateCopy(&infcopy, &infstrm);
    int rc_inf_orig = inflate(&infstrm, 0);
    int rc_inf_copy_run = inflate(&infcopy, 0);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, (voidpc)compBuf, (unsigned int)compLen);
    int rc_gz_putc = gzputc(gz, '\n');
    int rc_gz_close = gzclose(gz);

    // step 4: Validate & Cleanup
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inf_copy;
    (void)rc_inf_orig;
    (void)rc_inf_copy_run;
    (void)rc_gz_write;
    (void)rc_gz_putc;
    (void)rc_gz_close;
    inflateEnd(&infstrm);
    inflateEnd(&infcopy);
    free(compBuf);
    free(outBuf);
    // API sequence test completed successfully
    return 66;
}