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
//<ID> 467
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and allocate a buffer sized by compressBound
    const char src[] = "zlib API sequence payload: prepare -> deflateInit_ -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    memcpy(comp, source, (size_t)sourceLen);

    // step 2: Initialize a deflate stream and immediately end it to exercise init/end sequence
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Open a gz file, write the prepared buffer, use gzungetc and gzeof, then close
    gzFile gf = gzopen("zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)comp, (unsigned int)sourceLen);
    int rc_ungetc = gzungetc('A', gf);
    int rc_eof = gzeof(gf);
    int rc_flush = gzflush(gf, 1);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup
    free(comp);
    (void)version;
    (void)rc_def_init;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)rc_ungetc;
    (void)rc_eof;
    (void)rc_flush;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}