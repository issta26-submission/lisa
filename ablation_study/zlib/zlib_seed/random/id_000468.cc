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
//<ID> 468
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into an in-memory buffer
    const char src[] = "zlib API sequence payload: compress -> write gz -> reopen -> gzungetc/gzeof -> deflate init/end -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Write compressed data to a gzip file and close the writer
    gzFile gw = gzopen("zlib_api_seq_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gw, (voidpc)comp, (unsigned int)compLen);
    int rc_gw_close = gzclose(gw);

    // step 3: Reopen the gzip file for reading, push back a character, and query EOF state
    gzFile gr = gzopen("zlib_api_seq_tmp.gz", "rb");
    int rc_unget = gzungetc('X', gr);
    int rc_eof = gzeof(gr);
    int rc_gr_close = gzclose(gr);

    // step 4: Initialize and immediately clean up a deflate stream, then free allocated memory
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_end = deflateEnd(&dstrm);
    free(comp);

    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gw_close;
    (void)rc_unget;
    (void)rc_eof;
    (void)rc_gr_close;
    (void)rc_def_init;
    (void)rc_def_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}