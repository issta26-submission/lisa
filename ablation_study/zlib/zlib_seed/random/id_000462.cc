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
//<ID> 462
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src[] = "zlib API sequence payload: prepare -> deflate -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize and run a deflate stream using the compressed buffer as input
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = comp;
    dstrm.avail_in = (uInt)compLen;
    uLong outCap = sourceLen * 2 + 16;
    Bytef *out = (Bytef *)malloc((size_t)outCap);
    memset(out, 0, (size_t)outCap);
    dstrm.next_out = out;
    dstrm.avail_out = (uInt)outCap;
    int rc_def = deflate(&dstrm, 0);
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Write the deflated output to a gzip file, then reopen for reading and exercise gzungetc/gzeof
    gzFile gf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)out, (unsigned int)dstrm.total_out);
    int rc_gzclose1 = gzclose(gf);
    gzFile gf2 = gzopen("zlib_api_sequence_tmp.gz", "rb");
    int rc_gzungetc = gzungetc('A', gf2);
    int rc_gzeof = gzeof(gf2);
    int rc_gzclose2 = gzclose(gf2);

    // step 4: Cleanup
    free(comp);
    free(out);
    (void)version;
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)rc_gzclose1;
    (void)rc_gzungetc;
    (void)rc_gzeof;
    (void)rc_gzclose2;
    // API sequence test completed successfully
    return 66;
}