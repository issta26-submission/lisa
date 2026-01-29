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
//<ID> 157
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "zlib API sequence payload: compress -> deflate -> write gz -> inflateBackInit_ -> cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Setup z_streams and window for inflateBackInit_
    z_stream defstrm;
    z_stream backstrm;
    memset(&defstrm, 0, sizeof(z_stream));
    memset(&backstrm, 0, sizeof(z_stream));
    Bytef *defOut = (Bytef *)malloc((size_t)bound);
    memset(defOut, 0, (size_t)bound);
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, (size_t)32768);
    int rc_def_init = deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)source;
    defstrm.avail_in = (uInt)sourceLen;
    defstrm.next_out = defOut;
    defstrm.avail_out = (uInt)bound;

    // step 3: Core operations (deflate once, initialize inflateBack, write gz)
    int rc_deflate = deflate(&defstrm, 4); // 4 == Z_FINISH
    int rc_def_end = deflateEnd(&defstrm);
    int rc_inflate_back_init = inflateBackInit_(&backstrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_printf = gzprintf(gz, "zlib test");
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);
    int rc_gz_close = gzclose(gz);

    // step 4: Validate & Cleanup
    int rc_inflate_back_end = inflateBackEnd(&backstrm);
    free(compBuf);
    free(defOut);
    free(window);
    (void)rc_comp; (void)rc_def_init; (void)rc_deflate; (void)rc_def_end;
    (void)rc_inflate_back_init; (void)rc_gz_printf; (void)rc_gz_write; (void)rc_gz_close;
    (void)rc_inflate_back_end;
    // API sequence test completed successfully
    return 66;
}