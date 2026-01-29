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
//<ID> 531
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, query zlib version, and produce a compressed buffer with compress2
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> deflate -> inflateInit_ -> gzoffset64";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp2 = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize a deflate stream, compress the original source using deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    uLong defBound = deflateBound(&dstrm, sourceLen);
    Bytef *defOut = (Bytef *)malloc((size_t)defBound);
    memset(defOut, 0, (size_t)defBound);
    dstrm.next_out = defOut;
    dstrm.avail_out = (uInt)defBound;
    int rc_def = deflate(&dstrm, 4); // Z_FINISH
    uLong deflatedLen = (uLong)dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Initialize an inflate stream and decompress the buffer produced by compress2
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    uLongf destLen = (uLongf)(sourceLen + 64);
    Bytef *dest = (Bytef *)malloc((size_t)destLen);
    memset(dest, 0, (size_t)destLen);
    istrm.next_out = dest;
    istrm.avail_out = (uInt)destLen;
    int rc_inflate = inflate(&istrm, 4); // attempt finish
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Write the deflated output to a gzFile, query offset with gzoffset64, then cleanup
    gzFile gf = gzopen("test_zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)defOut, (unsigned int)deflatedLen);
    off64_t offset = gzoffset64(gf);
    int rc_gzclose = gzclose(gf);
    free(comp);
    free(defOut);
    free(dest);
    (void)version;
    (void)rc_comp2;
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_def_end;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)offset;
    // API sequence test completed successfully
    return 66;
}