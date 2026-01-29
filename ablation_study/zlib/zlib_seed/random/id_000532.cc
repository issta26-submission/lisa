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
//<ID> 532
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, query zlib version, and produce a compressed buffer with compress2
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> deflate -> inflate -> gzoffset64 -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize a deflate stream, run deflate on the original source, then cleanup deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong dOutBound = compressBound(sourceLen);
    Bytef *dout = (Bytef *)malloc((size_t)dOutBound);
    memset(dout, 0, (size_t)dOutBound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = dout;
    dstrm.avail_out = (uInt)dOutBound;
    int rc_def = deflate(&dstrm, 4);
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Initialize an inflate stream, decompress the buffer produced by compress2, then cleanup inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    uLongf destLen = (uLongf)(sourceLen + 64);
    Bytef *dest = (Bytef *)malloc((size_t)destLen);
    memset(dest, 0, (size_t)destLen);
    istrm.next_out = dest;
    istrm.avail_out = (uInt)destLen;
    int rc_inf = inflate(&istrm, 4);
    int rc_inf_end = inflateEnd(&istrm);

    // step 4: Write original data to a gzFile, query its 64-bit offset, close and cleanup all resources
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)source, (unsigned int)sourceLen);
    off64_t offset64 = gzoffset64(gf);
    int rc_gzclose = gzclose(gf);

    free(comp);
    free(dout);
    free(dest);

    (void)version;
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_def_end;
    (void)rc_inf_init;
    (void)rc_inf;
    (void)rc_inf_end;
    (void)rc_gzwrite;
    (void)offset64;
    (void)rc_gzclose;

    return 66;
    // API sequence test completed successfully
}