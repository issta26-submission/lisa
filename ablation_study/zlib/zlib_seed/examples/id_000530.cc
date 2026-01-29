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
//<ID> 530
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data, query zlib version, and create a compressed buffer via compress2
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> deflate -> inflate -> gzoffset64 -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize deflate and inflate streams
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Operate — deflate the original source, inflate the compress2 output, and use gzoffset64 on a gzFile
    uLong defBound = deflateBound(&dstrm, sourceLen);
    Bytef *defBuf = (Bytef *)malloc((size_t)defBound);
    memset(defBuf, 0, (size_t)defBound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = defBuf;
    dstrm.avail_out = (uInt)defBound;
    int rc_def = deflate(&dstrm, 4); /* Z_FINISH assumed as 4 */
    uLong defOutLen = dstrm.total_out;

    Bytef *dest = (Bytef *)malloc((size_t)(sourceLen + 64));
    memset(dest, 0, (size_t)(sourceLen + 64));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = dest;
    istrm.avail_out = (uInt)(sourceLen + 64);
    int rc_inf = inflate(&istrm, 0);
    uLong infOutLen = istrm.total_out;

    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)source, (unsigned int)sourceLen);
    off64_t offset64 = gzoffset64(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup resources
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    free(comp);
    free(defBuf);
    free(dest);
    (void)version;
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_inf_init;
    (void)rc_def;
    (void)defOutLen;
    (void)rc_inf;
    (void)infOutLen;
    (void)rc_gzwrite;
    (void)offset64;
    (void)rc_gzclose;
    (void)rc_def_end;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}