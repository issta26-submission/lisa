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
//<ID> 653
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compute compress bound and compress the data
    const char src[] = "zlib API sequence test payload for inflate and inflateBack initialization";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_compress = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize a deflate stream (configuration phase)
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 3: Persist compressed bytes to a gzFile and query its current offset
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)compLen);
    off_t gzpos = gztell(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Initialize inflate streams (inflateInit_ and inflateBackInit_), perform inflate, and cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf outAlloc = (uLongf)(sourceLen + 128);
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    z_stream back_strm;
    memset(&back_strm, 0, sizeof(z_stream));
    int rc_inflate_back_init = inflateBackInit_(&back_strm, 15, window, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&istrm, 4);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_inflate_back_end = inflateBackEnd(&back_strm);
    int rc_def_end = deflateEnd(&dstrm);

    free(compBuf);
    free(outBuf);
    free(window);

    (void)rc_compress;
    (void)rc_def_init;
    (void)rc_gzwrite;
    (void)gzpos;
    (void)rc_gzclose;
    (void)rc_inflate_init;
    (void)rc_inflate_back_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_inflate_back_end;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}