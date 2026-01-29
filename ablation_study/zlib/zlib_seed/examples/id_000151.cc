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
//<ID> 151
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
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    z_stream defstrm;
    z_stream backstrm;
    memset(&defstrm, 0, sizeof(z_stream));
    memset(&backstrm, 0, sizeof(z_stream));

    // step 2: Setup & Compress
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 3: Core operations (deflate one pass, initialize inflate-back, write gz output)
    Bytef *defOutBuf = (Bytef *)malloc((size_t)bound);
    memset(defOutBuf, 0, (size_t)bound);
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)source;
    defstrm.avail_in = (uInt)sourceLen;
    defstrm.next_out = defOutBuf;
    defstrm.avail_out = (uInt)bound;
    int rc_deflate = deflate(&defstrm, 0);
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_inflate_back_init = inflateBackInit_(&backstrm, 15, window, version, (int)sizeof(z_stream));
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_printf = gzprintf(gz, "Compressed %lu -> wrote to gz\n", (unsigned long)compLen);
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);

    // step 4: Validate & Cleanup
    int rc_def_end = deflateEnd(&defstrm);
    int rc_inflate_back_end = inflateBackEnd(&backstrm);
    int rc_gz_close = gzclose(gz);
    free(compBuf);
    free(defOutBuf);
    free(window);
    (void)rc_comp; (void)rc_def_init; (void)rc_deflate; (void)rc_inflate_back_init;
    (void)rc_gz_printf; (void)rc_gz_write; (void)rc_def_end; (void)rc_inflate_back_end; (void)rc_gz_close;
    // API sequence test completed successfully
    return 66;
}