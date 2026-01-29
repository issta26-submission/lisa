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
//<ID> 538
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, query version, compress with compress2
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> inflateInit_ -> deflate -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp2 = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream and decompress the buffer produced by compress2
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    uLongf destLen = (uLongf)(sourceLen + 128);
    Bytef *dest = (Bytef *)malloc((size_t)destLen);
    memset(dest, 0, (size_t)destLen);
    istrm.next_out = dest;
    istrm.avail_out = (uInt)destLen;
    int rc_inflate = inflate(&istrm, 0);

    // step 3: Initialize deflate stream, compress the inflated data with deflate, then finalize
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = dest;
    dstrm.avail_in = (uInt)destLen;
    uLong comp2Bound = deflateBound(&dstrm, (uLong)destLen);
    Bytef *comp2 = (Bytef *)malloc((size_t)comp2Bound);
    memset(comp2, 0, (size_t)comp2Bound);
    dstrm.next_out = comp2;
    dstrm.avail_out = (uInt)comp2Bound;
    int rc_deflate = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    int rc_def_end = deflateEnd(&dstrm);

    // step 4: Write the deflated buffer to a gzFile, query its 64-bit offset, then cleanup
    gzFile gf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)comp2, (unsigned int)dstrm.total_out);
    off64_t off = gzoffset64(gf);
    int rc_gzclose = gzclose(gf);

    /* cleanup inflate stream and buffers */
    int rc_inflate_end = inflateEnd(&istrm);
    free(comp);
    free(dest);
    free(comp2);

    /* silence unused-return warnings while preserving meaningful data flow */
    (void)version;
    (void)rc_comp2;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)off;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}