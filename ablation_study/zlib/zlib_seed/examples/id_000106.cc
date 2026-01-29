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
//<ID> 106
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
    const char src_c[] = "Example payload for zlib: compress -> gzwrite/gzputc -> inflateCopy -> inflate";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    const char *version = zlibVersion();

    // step 2: Setup (compress source and init inflate stream, then copy it)
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    int rc_inf_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    int rc_inf_copy = inflateCopy(&infcopy, &infstrm);

    // step 3: Core operations (write compressed data to gz using gzwrite and gzputc, then inflate using both original and copied streams)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);
    int rc_gz_putc = gzputc(gz, '\n');
    Bytef *outBuf1 = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf1, 0, (size_t)sourceLen);
    Bytef *outBuf2 = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf2, 0, (size_t)sourceLen);
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)compLen;
    infstrm.next_out = outBuf1;
    infstrm.avail_out = (uInt)sourceLen;
    int rc_inf1 = inflate(&infstrm, 0);
    infcopy.next_in = compBuf;
    infcopy.avail_in = (uInt)compLen;
    infcopy.next_out = outBuf2;
    infcopy.avail_out = (uInt)sourceLen;
    int rc_inf2 = inflate(&infcopy, 0);

    // step 4: Cleanup & finalize
    (void)rc_comp; (void)rc_inf_init; (void)rc_inf_copy; (void)rc_gz_write; (void)rc_gz_putc; (void)rc_inf1; (void)rc_inf2;
    gzclose(gz);
    inflateEnd(&infcopy);
    inflateEnd(&infstrm);
    free(compBuf);
    free(outBuf1);
    free(outBuf2);
    // API sequence test completed successfully
    return 66;
}