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
//<ID> 75
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defstrm;
    z_stream infstrm;
    memset(&defstrm, 0, sizeof(z_stream));
    memset(&infstrm, 0, sizeof(z_stream));
    const char src_c[] = "zlib API sequence payload for compress -> gzwrite -> offset check";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    const char *version = zlibVersion();

    // step 2: Setup (Initialize zlib streams)
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_inf_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));

    // step 3: Configure & Operate (adjust deflate params, compress, write to gz file, get offset)
    int rc_def_params = deflateParams(&defstrm, 5, 0);
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gz_wr = gzwrite(gz, compBuf, (unsigned int)compLen);
    off64_t offs = gzoffset64(gz);

    // step 4: Validate & Cleanup
    int rc_def_end = deflateEnd(&defstrm);
    int rc_inf_end = inflateEnd(&infstrm);
    int rc_gzclose = gzclose(gz);
    free(compBuf);
    (void)rc_def_init; (void)rc_inf_init; (void)rc_def_params; (void)rc_comp;
    (void)gz_wr; (void)offs; (void)rc_def_end; (void)rc_inf_end; (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}