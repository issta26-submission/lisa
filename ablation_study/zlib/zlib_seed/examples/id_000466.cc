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
//<ID> 466
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src[] = "zlib API sequence payload: compress -> deflateInit_ -> gz write/read -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize a deflate stream (to demonstrate init/configure)
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 3: Write compressed data to a gzip file, reopen for reading and use gzungetc/gzeof
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gw, (voidpc)comp, (unsigned int)compLen);
    int rc_gzclose_w = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    int first_byte = (int)comp[0];
    int rc_ungetc = gzungetc(first_byte, gr);
    int eof_flag = gzeof(gr);
    int rc_gzclose_r = gzclose(gr);

    // step 4: Cleanup and finalize deflate stream, free resources
    int rc_def_end = deflateEnd(&dstrm);
    free(comp);
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_ungetc;
    (void)eof_flag;
    (void)rc_gzclose_r;
    (void)rc_def_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}