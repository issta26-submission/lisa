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
//<ID> 79
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream infstrm;
    z_stream defstrm;
    memset(&infstrm, 0, sizeof(z_stream));
    memset(&defstrm, 0, sizeof(z_stream));
    const char src_c[] = "Sample payload for zlib API sequence: compress -> write gz -> check offset";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    const char *version = zlibVersion();

    // step 2: Setup (initialize inflate and deflate states, configure deflate)
    int rc_inf_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_params = deflateParams(&defstrm, 5, 0);

    // step 3: Operate (compress the source, write compressed bytes to a .gz file, get offset)
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);
    off64_t pos64 = gzoffset64(gz);

    // step 4: Validate & Cleanup
    (void)rc_inf_init; (void)rc_def_init; (void)rc_def_params; (void)rc_comp; (void)rc_gz_write; (void)pos64;
    gzclose(gz);
    deflateEnd(&defstrm);
    inflateEnd(&infstrm);
    free(compBuf);
    // API sequence test completed successfully
    return 66;
}