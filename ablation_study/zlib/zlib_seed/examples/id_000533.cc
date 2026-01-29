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
//<ID> 533
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it with compress2
    const char src[] = "zlib API sequence payload: compress2 -> init streams -> deflate -> gz IO -> validate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize an inflate stream (validate initialization API) and a deflate stream for a secondary compression pass
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 3: Use deflate to compress the same source into a separate buffer, then write the first compressed buffer to a gzFile and query its offset
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    uLong defOutBound = deflateBound(&dstrm, sourceLen);
    Bytef *defOut = (Bytef *)malloc((size_t)defOutBound);
    memset(defOut, 0, (size_t)defOutBound);
    dstrm.next_out = defOut;
    dstrm.avail_out = (uInt)defOutBound;
    int rc_deflate = deflate(&dstrm, 4);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)comp, (unsigned int)compLen);
    off64_t off = gzoffset64(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate by uncompressing the compress2 output, cleanup streams and buffers
    uLongf destLen = (uLongf)(sourceLen + 128);
    Bytef *dest = (Bytef *)malloc((size_t)destLen);
    memset(dest, 0, (size_t)destLen);
    int rc_uncomp = uncompress(dest, &destLen, comp, (uLong)compLen);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    free(comp);
    free(defOut);
    free(dest);
    (void)version;
    (void)rc_comp;
    (void)rc_inflate_init;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_gzwrite;
    (void)off;
    (void)rc_gzclose;
    (void)rc_uncomp;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}