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
//<ID> 152
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "zlib API sequence payload: compress -> deflate -> write gz -> inflateBackInit_";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    z_stream dstrm;
    z_stream backstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    memset(&backstrm, 0, sizeof(z_stream));

    // step 2: Compress with compress() and configure deflate stream
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    int rc_def_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compLen;
    int rc_def = deflate(&dstrm, 0);
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Operate (write compressed data to gz file using gzprintf and gzwrite)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_printf = gzprintf(gz, "zlib-sequence-header\n");
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);
    int rc_gz_close = gzclose(gz);

    // step 4: Validate & Cleanup (initialize inflateBack, teardown, free buffers)
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_inf_back_init = inflateBackInit_(&backstrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    int rc_inf_back_end = inflateBackEnd(&backstrm);
    free(compBuf);
    free(window);

    (void)rc_comp; (void)rc_def_init; (void)rc_def; (void)rc_def_end;
    (void)rc_gz_printf; (void)rc_gz_write; (void)rc_gz_close;
    (void)rc_inf_back_init; (void)rc_inf_back_end; (void)compLen; (void)bound; (void)sourceLen;
    // API sequence test completed successfully
    return 66;
}