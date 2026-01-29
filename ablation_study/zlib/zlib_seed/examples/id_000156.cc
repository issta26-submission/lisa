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
//<ID> 156
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize buffers and source
    const char src_c[] = "zlib API sequence payload: compress -> deflate -> gz write -> inflateBackInit";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    const char *version = zlibVersion();

    // step 2: Compress and write to gzip file (use compress, gzprintf, gzwrite)
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_printf = gzprintf(gz, "Compressed size: %lu\n", (unsigned long)compLen);
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);
    int rc_gz_close = gzclose(gz);

    // step 3: Initialize deflate stream and perform a deflate step
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    Bytef *defOut = (Bytef *)malloc((size_t)bound);
    memset(defOut, 0, (size_t)bound);
    dstrm.next_out = defOut;
    dstrm.avail_out = (uInt)bound;
    int rc_def = deflate(&dstrm, 0);
    int rc_def_end = deflateEnd(&dstrm);

    // step 4: Initialize inflate-backend and cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(1);
    memset(window, 0, 1);
    int rc_infback_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    int rc_infback_end = inflateBackEnd(&istrm);

    free(compBuf);
    free(defOut);
    free(window);

    (void)rc_comp; (void)rc_gz_printf; (void)rc_gz_write; (void)rc_gz_close;
    (void)rc_def_init; (void)rc_def; (void)rc_def_end; (void)rc_infback_init; (void)rc_infback_end;
    (void)compLen; (void)sourceLen; (void)bound;
    // API sequence test completed successfully
    return 66;
}