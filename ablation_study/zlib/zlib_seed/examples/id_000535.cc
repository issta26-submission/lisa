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
//<ID> 535
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, query version, and produce a compressed buffer with compress2
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> deflate -> gz IO -> inflate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize a deflate stream, compute bound, and run deflate on the original source
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong defBound = deflateBound(&dstrm, sourceLen);
    Bytef *defbuf = (Bytef *)malloc((size_t)defBound);
    memset(defbuf, 0, (size_t)defBound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = defbuf;
    dstrm.avail_out = (uInt)defBound;
    int rc_def = deflate(&dstrm, Z_FINISH);

    // step 3: Write the compressed buffer produced by compress2 to a gzFile and query its offset with gzoffset64
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)comp, (unsigned int)compLen);
    off64_t offset = gzoffset64(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Initialize an inflate stream, decompress the compress2 output, cleanup, and return
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLongf destLen = (uLongf)(sourceLen + 128);
    Bytef *dest = (Bytef *)malloc((size_t)destLen);
    memset(dest, 0, (size_t)destLen);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = dest;
    istrm.avail_out = (uInt)destLen;
    int rc_inf = inflate(&istrm, Z_FINISH);

    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    free(comp);
    free(defbuf);
    free(dest);
    (void)version;
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_gzwrite;
    (void)offset;
    (void)rc_gzclose;
    (void)rc_inf_init;
    (void)rc_inf;
    (void)rc_def_end;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}