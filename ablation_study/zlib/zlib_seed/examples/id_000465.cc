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
//<ID> 465
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and produce a one-shot compressed buffer
    const char src[] = "zlib API sequence payload: prepare -> stream deflate -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize and configure a deflate stream for streaming compression
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_params = deflateParams(&dstrm, 6, 0);
    uLong outBound = deflateBound(&dstrm, sourceLen);
    Bytef *out = (Bytef *)malloc((size_t)outBound);
    memset(out, 0, (size_t)outBound);
    dstrm.next_in = comp;
    dstrm.avail_in = (uInt)compLen;
    dstrm.next_out = out;
    dstrm.avail_out = (uInt)outBound;

    // step 3: Operate (stream deflate) and perform gz-file IO and simple validation calls
    int rc_def = deflate(&dstrm, 0);
    gzFile gf = gzopen("zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)out, (unsigned int)dstrm.total_out);
    int rc_ungetc = gzungetc((int)'A', gf);
    int rc_eof = gzeof(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup resources and finalize deflate stream
    int rc_def_end = deflateEnd(&dstrm);
    free(comp);
    free(out);
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_params;
    (void)rc_def;
    (void)rc_gzwrite;
    (void)rc_ungetc;
    (void)rc_eof;
    (void)rc_gzclose;
    (void)rc_def_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}