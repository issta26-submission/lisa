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
//<ID> 536
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and produce a compressed buffer with compress2
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> gzoffset64 -> inflateInit_ -> deflate";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Open a gzFile, write original data to it and query its 64-bit offset
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)source, (unsigned int)sourceLen);
    off64_t offset64 = gzoffset64(gf);
    int rc_gzclose = gzclose(gf);

    // step 3: Initialize an inflate stream, set input to the compressed buffer, and inflate to dest
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    uLongf destLen = (uLongf)(sourceLen + 128);
    Bytef *dest = (Bytef *)malloc((size_t)destLen);
    memset(dest, 0, (size_t)destLen);
    istrm.next_out = dest;
    istrm.avail_out = (uInt)destLen;
    int rc_in_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&istrm, 4); // Z_FINISH == 4
    int rc_in_end = inflateEnd(&istrm);

    // step 4: Initialize a deflate stream and compress the inflated output, then cleanup
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = dest;
    dstrm.avail_in = (uInt)istrm.total_out; // bytes produced by inflate
    uLong outBound = deflateBound(&dstrm, (uLong)dstrm.avail_in);
    Bytef *outbuf = (Bytef *)malloc((size_t)outBound);
    memset(outbuf, 0, (size_t)outBound);
    dstrm.next_out = outbuf;
    dstrm.avail_out = (uInt)outBound;
    int rc_deflate = deflate(&dstrm, 4); // Z_FINISH == 4
    int rc_def_end = deflateEnd(&dstrm);

    free(comp);
    free(dest);
    free(outbuf);
    (void)version;
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)offset64;
    (void)rc_gzclose;
    (void)rc_in_init;
    (void)rc_inflate;
    (void)rc_in_end;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}