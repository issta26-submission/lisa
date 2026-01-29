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
//<ID> 469
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and obtain compress bound, then compress into a buffer
    const char src[] = "zlib API sequence payload: prepare -> compressBound -> compress -> deflate -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize a deflate stream and configure it for a secondary compression pass
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = comp;
    dstrm.avail_in = (uInt)compLen;
    uLong outBound = deflateBound(&dstrm, (uLong)compLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;

    // step 3: Operate the deflate stream, write results to a gzFile, then perform gzungetc and gzeof checks
    int rc_def_op = deflate(&dstrm, 0);
    gzFile gf = gzopen("zlib_api_sequence_tmp.gz", "wb+");
    int rc_gzwrite = 0;
    if (gf) {
        rc_gzwrite = gzwrite(gf, (voidpc)outBuf, (unsigned int)dstrm.total_out);
    }
    int rc_ungetc = gzungetc((int)(unsigned char)(outBuf[0]), gf);
    int rc_eof = gzeof(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup deflate stream and memory
    int rc_def_end = deflateEnd(&dstrm);
    free(comp);
    free(outBuf);
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_def_op;
    (void)rc_gzwrite;
    (void)rc_ungetc;
    (void)rc_eof;
    (void)rc_gzclose;
    (void)rc_def_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}