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
//<ID> 512
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and produce a compressed buffer with compress2
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> deflateInit_ -> deflate -> uncompress -> gzwrite -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize a deflate stream, compress the original source into an out buffer, then finish deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong outBound = deflateBound(&dstrm, sourceLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;
    int rc_def = deflate(&dstrm, 0);
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Uncompress the compress2 output back into a destination buffer, write the result to a gzFile
    uLongf destLen = (uLongf)(sourceLen + 64);
    Bytef *dest = (Bytef *)malloc((size_t)destLen);
    memset(dest, 0, (size_t)destLen);
    int rc_uncomp = uncompress(dest, &destLen, comp, (uLong)compLen);
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)dest, (unsigned int)destLen);
    int rc_gzclose = gzclose(gf);

    // step 4: Query error text for the uncompress return, obtain zlib version again for completeness, cleanup
    const char *err_text = zError(rc_uncomp);
    const char *ver_again = zlibVersion();
    free(comp);
    free(outBuf);
    free(dest);
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_def_end;
    (void)rc_uncomp;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)err_text;
    (void)ver_again;
    // API sequence test completed successfully
    return 66;
}