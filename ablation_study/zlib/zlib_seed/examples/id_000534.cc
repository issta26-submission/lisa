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
//<ID> 534
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, query zlib version, and compress with compress2
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> deflate -> inflate -> gz offset -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize a deflate stream and perform a streaming deflate of the original data
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong dBound = deflateBound(&dstrm, sourceLen);
    Bytef *dcomp = (Bytef *)malloc((size_t)dBound);
    memset(dcomp, 0, (size_t)dBound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = dcomp;
    dstrm.avail_out = (uInt)dBound;
    int rc_def = deflate(&dstrm, 4);
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Initialize an inflate stream and inflate the buffer produced by compress2; use gz operations and query gzoffset64
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLongf destLen = (uLongf)(sourceLen + 64);
    Bytef *dest = (Bytef *)malloc((size_t)destLen);
    memset(dest, 0, (size_t)destLen);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = dest;
    istrm.avail_out = (uInt)destLen;
    int rc_inf = inflate(&istrm, 4);
    int rc_inf_end = inflateEnd(&istrm);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)source, (unsigned int)sourceLen);
    off64_t offset = gzoffset64(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup all allocated resources
    free(comp);
    free(dcomp);
    free(dest);
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_def_end;
    (void)rc_inf_init;
    (void)rc_inf;
    (void)rc_inf_end;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)offset;
    (void)version;
    // API sequence test completed successfully
    return 66;
}