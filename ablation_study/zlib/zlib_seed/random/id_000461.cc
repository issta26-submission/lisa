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
//<ID> 461
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and perform a first-stage compression into a buffer
    const char src[] = "zlib API sequence payload: prepare -> compress -> deflateInit_ -> deflateBound -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize a deflate stream, query a bound for further compression, then end the deflate stream
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong second_bound = deflateBound(&dstrm, sourceLen);
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Write the compressed buffer to a gzip file, read a byte back, push it back with gzungetc, and check EOF
    gzFile gw = gzopen("zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gw, (voidpc)comp, (unsigned int)compLen);
    int rc_gwclose = gzclose(gw);
    gzFile gr = gzopen("zlib_api_sequence_out.gz", "rb");
    int c = gzgetc(gr);
    int rc_unget = gzungetc(c, gr);
    int rc_eof = gzeof(gr);
    int rc_grclose = gzclose(gr);

    // step 4: Cleanup all resources and return success
    free(comp);
    (void)source;
    (void)rc_comp;
    (void)version;
    (void)rc_def_init;
    (void)second_bound;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)rc_gwclose;
    (void)c;
    (void)rc_unget;
    (void)rc_eof;
    (void)rc_grclose;
    // API sequence test completed successfully
    return 66;
}