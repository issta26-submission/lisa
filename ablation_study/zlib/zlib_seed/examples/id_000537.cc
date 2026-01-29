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
//<ID> 537
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data, query version, and produce a compressed buffer with compress2
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> deflate -> gz IO -> inflate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp2 = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize a deflate stream, feed it the original data, call deflate, and finish
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    Bytef *defOut = (Bytef *)malloc((size_t)compBound);
    memset(defOut, 0, (size_t)compBound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = defOut;
    dstrm.avail_out = (uInt)compBound;
    int rc_def = deflate(&dstrm, 4); // 4 == Z_FINISH
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Create a gzFile, write the original data, query gzoffset64, and close
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (const void*)source, (unsigned int)sourceLen);
    off64_t offset64 = gzoffset64(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Initialize an inflate stream, set compressed input (from compress2), inflate to output buffer, cleanup
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
    int rc_inflate = inflate(&istrm, 4); // 4 == Z_FINISH
    int rc_inf_end = inflateEnd(&istrm);

    free(comp);
    free(defOut);
    free(dest);
    (void)rc_comp2;
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)offset64;
    (void)rc_gzclose;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}