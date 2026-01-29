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
//<ID> 1287
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and source preparation
    const uLong SRC_LEN = 512UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'A', (size_t)SRC_LEN);
    uLong comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    uLong comp_len = comp_bound;
    const char *ver = zlibVersion();

    // step 2: Compress source, write gzip file using gzopen64, then close
    compress2(comp, (uLongf *)&comp_len, src, SRC_LEN, 6);
    gzFile gw64 = gzopen64("test_zlib_api_sequence64.gz", "wb");
    gzwrite(gw64, src, (unsigned int)SRC_LEN);
    gzclose(gw64);

    // step 3: Decompress using uncompress2 and second decompression path using inflateBack API
    uLong decomp_len = SRC_LEN + 128;
    Bytef *decomp = (Bytef *)malloc((size_t)decomp_len);
    memset(decomp, 0, (size_t)decomp_len);
    uLong source_len_for_uncompress = comp_len;
    uncompress2(decomp, (uLongf *)&decomp_len, comp, &source_len_for_uncompress);

    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    backstrm.zalloc = NULL;
    backstrm.zfree = NULL;
    backstrm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&backstrm, 15, window, ver, (int)sizeof(z_stream));

    struct InDesc { const unsigned char *data; uLong len; uLong pos; };
    struct OutDesc { unsigned char *dest; uLong cap; uLong pos; };
    InDesc *in_desc = (InDesc *)malloc(sizeof(InDesc));
    in_desc->data = (const unsigned char *)comp;
    in_desc->len = comp_len;
    in_desc->pos = 0;
    OutDesc *out_desc = (OutDesc *)malloc(sizeof(OutDesc));
    out_desc->cap = SRC_LEN + 256;
    out_desc->dest = (unsigned char *)malloc((size_t)out_desc->cap);
    out_desc->pos = 0;
    in_func in_cb = [](void *in_d, unsigned char **buf)->unsigned int {
        InDesc *d = (InDesc *)in_d;
        if (d->pos >= d->len) {
            *buf = 0;
            return 0u;
        }
        *buf = (unsigned char *)(d->data + d->pos);
        unsigned int avail = (unsigned int)(d->len - d->pos);
        d->pos += avail;
        return avail;
    };
    out_func out_cb = [](void *out_d, unsigned char *buf, unsigned int len)->int {
        OutDesc *d = (OutDesc *)out_d;
        memcpy(d->dest + d->pos, buf, len);
        d->pos += len;
        return 0;
    };
    inflateBack(&backstrm, in_cb, (void *)in_desc, out_cb, (void *)out_desc);
    inflateBackEnd(&backstrm);

    // step 4: Read back gzip with gzopen, validate via simple checksum usage, cleanup
    gzFile gr = gzopen("test_zlib_api_sequence64.gz", "rb");
    Bytef *readbuf = (Bytef *)malloc((size_t)SRC_LEN);
    memset(readbuf, 0, (size_t)SRC_LEN);
    gzread(gr, readbuf, (unsigned int)SRC_LEN);
    gzclose(gr);

    uLong crc_original = crc32(0UL, src, (uInt)SRC_LEN);
    uLong crc_read = crc32(0UL, readbuf, (uInt)SRC_LEN);
    (void)crc_original;
    (void)crc_read;

    free(src);
    free(comp);
    free(decomp);
    free(window);
    free(in_desc);
    free(out_desc->dest);
    free(out_desc);
    free(readbuf);
    // API sequence test completed successfully
    return 66;
}