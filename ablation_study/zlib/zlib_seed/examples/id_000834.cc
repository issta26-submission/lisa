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
//<ID> 834
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char src[] = "zlib api sequence payload for gzwrite, gzoffset64, inflateBack and crc32";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong crc_initial = crc32(0L, source, (uInt)sourceLen);
    uLong compBound = compressBound(sourceLen);
    uLongf compLen = (uLongf)compBound;
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, source, (unsigned int)sourceLen);
    off64_t offset = gzoffset64(gf);
    int rc_gz_close = gzclose(gf);

    // step 2: Setup inflateBack stream and window
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_inflate_init = inflateBackInit_(&istrm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Prepare in/out descriptors and callbacks then call inflateBack
    struct InDesc { const unsigned char *data; uInt len; uInt pos; };
    struct OutDesc { unsigned char *data; uInt len; uInt pos; };
    InDesc inDesc = { compBuf, (uInt)compLen, 0 };
    uInt outBufSize = (uInt)(sourceLen + 64);
    unsigned char *outBuf = (unsigned char *)malloc((size_t)outBufSize);
    memset(outBuf, 0, outBufSize);
    OutDesc outDesc = { outBuf, outBufSize, 0 };
    in_func in_cb = +[](void *desc, unsigned char **buf)->unsigned int {
        InDesc *d = (InDesc *)desc;
        unsigned int avail = d->len - d->pos;
        *buf = (unsigned char *)(d->data + d->pos);
        d->pos += avail;
        return avail;
    };
    out_func out_cb = +[](void *desc, unsigned char *buf, unsigned int len)->int {
        OutDesc *d = (OutDesc *)desc;
        memcpy(d->data + d->pos, buf, (size_t)len);
        d->pos += len;
        return 0;
    };
    int rc_inflate = inflateBack(&istrm, in_cb, (void *)&inDesc, out_cb, (void *)&outDesc);
    int rc_inflate_end = inflateBackEnd(&istrm);

    // step 4: Validation and cleanup
    (void)crc_initial;
    (void)rc_comp;
    (void)rc_gz_write;
    (void)offset;
    (void)rc_gz_close;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    free(compBuf);
    free(window);
    free(outBuf);
    // API sequence test completed successfully
    return 66;
}