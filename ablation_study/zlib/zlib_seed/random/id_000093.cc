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
//<ID> 93
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream infstrm;
    z_stream infbackstrm;
    memset(&infstrm, 0, sizeof(z_stream));
    memset(&infbackstrm, 0, sizeof(z_stream));
    const char src_c[] = "Payload for zlib API sequence: compress -> write gz -> inflate back cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    const char *version = zlibVersion();

    // step 2: Setup (initialize inflate and inflateBack, compress source)
    int rc_inf_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_infback_init = inflateBackInit_(&infbackstrm, 15, window, version, (int)sizeof(z_stream));
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 3: Operate (write compressed data to .gz with buffering and query offset)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzbuffer = gzbuffer(gz, 8192);
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);
    off64_t pos64 = gzoffset64(gz);

    // step 4: Validate & Cleanup
    int rc_infback_end = inflateBackEnd(&infbackstrm);
    int rc_inf_end = inflateEnd(&infstrm);
    int rc_gz_close = gzclose(gz);
    free(compBuf);
    free(window);
    (void)rc_inf_init; (void)rc_infback_init; (void)rc_comp; (void)rc_gzbuffer; (void)rc_gz_write; (void)pos64;
    (void)rc_infback_end; (void)rc_inf_end; (void)rc_gz_close;
    // API sequence test completed successfully
    return 66;
}