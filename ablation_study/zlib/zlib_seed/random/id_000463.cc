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
//<ID> 463
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src[] = "zlib API sequence payload: prepare -> compressBound -> compress -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize and configure a deflate stream (to demonstrate lifecycle)
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 3: Operate with gz file I/O using the compressed buffer and test gzungetc/gzeof
    gzFile gf = gzopen("zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)comp, (unsigned int)compLen);
    int rc_gzungetc = gzungetc('A', gf);
    int rc_gzeof = gzeof(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup deflate stream and buffers
    int rc_def_end = deflateEnd(&dstrm);
    free(comp);
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_gzwrite;
    (void)rc_gzungetc;
    (void)rc_gzeof;
    (void)rc_gzclose;
    (void)rc_def_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}