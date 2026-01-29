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
//<ID> 1280
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, compress it, and write/read gzip files using gzopen64 and gzopen
    const uLong SRC_LEN = 512UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'A', (size_t)SRC_LEN);
    uLongf comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    uLongf comp_len_f = comp_bound;
    const char *ver = zlibVersion();
    compress2(comp, &comp_len_f, src, SRC_LEN, 6);
    gzFile gw64 = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw64, src, (unsigned int)SRC_LEN);
    gzclose(gw64);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *readbuf = (Bytef *)malloc((size_t)SRC_LEN);
    memset(readbuf, 0, (size_t)SRC_LEN);
    gzread(gr, readbuf, (unsigned int)SRC_LEN);
    gzclose(gr);

    // step 2: Use uncompress2 to decompress the compressed data into a buffer and validate sizes flow
    uLong sourceLen_for_uncompress = (uLong)comp_len_f;
    uLongf decomp_cap = SRC_LEN + 64;
    Bytef *decomp = (Bytef *)malloc((size_t)decomp_cap);
    memset(decomp, 0, (size_t)decomp_cap);
    uncompress2(decomp, &decomp_cap, comp, &sourceLen_for_uncompress);

    // step 3: Initialize inflateBack via inflateBackInit_, provide a window, and run inflateBack with callbacks
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    backstrm.zalloc = NULL;
    backstrm.zfree = NULL;
    backstrm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&backstrm, 15, window, ver, (int)sizeof(z_stream));
    struct InDesc { Bytef *data; uLong length; uLong pos; };
    struct OutDesc { Bytef *out; uLong capacity; uLong pos; };
    InDesc in_desc;
    in_desc.data = comp;
    in_desc.length = (uLong)comp_len_f;
    in_desc.pos = 0;
    OutDesc out_desc;
    out_desc.out = (Bytef *)malloc((size_t)(SRC_LEN + 128));
    memset(out_desc.out, 0, (size_t)(SRC_LEN + 128));
    out_desc.capacity = SRC_LEN + 128;
    out_desc.pos = 0;
    unsigned int (*in_cb)(void *, unsigned char **) = +[](void *in_d, unsigned char **buf)->unsigned int {
        InDesc *d = (InDesc *)in_d;
        unsigned int rem = (unsigned int)(d->length - d->pos);
        *buf = (unsigned char *)(d->data + d->pos);
        d->pos += rem;
        return rem;
    };
    int (*out_cb)(void *, unsigned char *, unsigned int) = +[](void *out_d, unsigned char *buf, unsigned int len)->int {
        OutDesc *o = (OutDesc *)out_d;
        memcpy(o->out + o->pos, buf, (size_t)len);
        o->pos += len;
        return 0;
    };
    inflateBack(&backstrm, (in_func)in_cb, &in_desc, (out_func)out_cb, &out_desc);
    inflateBackEnd(&backstrm);

    // step 4: Cleanup all resources
    free(src);
    free(comp);
    free(decomp);
    free(readbuf);
    free(window);
    free(out_desc.out);
    // API sequence test completed successfully
    return 66;
}