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
//<ID> 153
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "zlib API sequence payload: compress -> deflate -> write gz -> inflateBackInit";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();

    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    z_stream defstrm;
    z_stream backstrm;
    memset(&defstrm, 0, sizeof(z_stream));
    memset(&backstrm, 0, sizeof(z_stream));

    size_t window_size = 1u << 15; /* 32K window */
    unsigned char *window = (unsigned char *)malloc(window_size);
    memset(window, 0, window_size);

    // step 2: Setup & Compress using compress()
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    /* prepare a secondary output buffer for deflate operation */
    Bytef *defOut = (Bytef *)malloc((size_t)bound);
    memset(defOut, 0, (size_t)bound);

    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)source;
    defstrm.avail_in = (uInt)sourceLen;
    defstrm.next_out = defOut;
    defstrm.avail_out = (uInt)bound;
    int rc_deflate = deflate(&defstrm, 0);

    // step 3: Operate (write compressed data to gz file and initialize inflateBack)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_printf = gzprintf(gz, "zlib-seq");
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);
    int rc_gz_close = gzclose(gz);

    int rc_inflateBackInit = inflateBackInit_(&backstrm, 15, window, version, (int)sizeof(z_stream));

    // step 4: Validate & Cleanup
    int rc_def_end = deflateEnd(&defstrm);
    int rc_back_end = inflateBackEnd(&backstrm);
    free(compBuf);
    free(defOut);
    free(window);

    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_gz_printf;
    (void)rc_gz_write;
    (void)rc_gz_close;
    (void)rc_inflateBackInit;
    (void)rc_def_end;
    (void)rc_back_end;
    // API sequence test completed successfully
    return 66;
}