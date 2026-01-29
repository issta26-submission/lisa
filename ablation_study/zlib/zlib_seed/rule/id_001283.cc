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
//<ID> 1283
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source buffer and allocate working buffers
    const uLong SRC_LEN = 512UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'A', (size_t)SRC_LEN);
    uLongf comp_bound = (uLongf)compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    uLongf comp_len = comp_bound;
    const char *ver = zlibVersion();
    compress2(comp, &comp_len, src, SRC_LEN, 6);

    // step 2: Use uncompress2 to validate round-trip and prepare inflateBack input state
    uLongf decomp_len = SRC_LEN + 64;
    Bytef *decomp = (Bytef *)malloc((size_t)decomp_len);
    memset(decomp, 0, (size_t)decomp_len);
    uLong comp_len_for_uncompress = (uLong)comp_len;
    uncompress2(decomp, &decomp_len, comp, &comp_len_for_uncompress);

    // step 3: Initialize inflateBack and run it with simple callbacks that stream comp -> back_decomp
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    backstrm.zalloc = NULL;
    backstrm.zfree = NULL;
    backstrm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&backstrm, 15, window, ver, (int)sizeof(z_stream));

    struct InDesc { const unsigned char *data; uLong size; uLong offset; };
    struct OutDesc { unsigned char *buf; uLong size; uLong offset; };
    InDesc in_desc;
    in_desc.data = comp;
    in_desc.size = (uLong)comp_len;
    in_desc.offset = 0;
    OutDesc out_desc;
    out_desc.buf = (unsigned char *)malloc((size_t)(SRC_LEN + 128));
    out_desc.size = SRC_LEN + 128;
    out_desc.offset = 0;

    in_func in_cb = (in_func) +[](void *v, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)v;
        unsigned int remaining = (unsigned int)(d->size - d->offset);
        *buf = (unsigned char *)(d->data + d->offset);
        d->offset += remaining;
        return remaining;
    };
    out_func out_cb = (out_func) +[](void *v, unsigned char *buf, unsigned int len) -> int {
        OutDesc *o = (OutDesc *)v;
        memcpy(o->buf + o->offset, buf, (size_t)len);
        o->offset += (uLong)len;
        return 0;
    };

    inflateBack(&backstrm, in_cb, &in_desc, out_cb, &out_desc);
    inflateBackEnd(&backstrm);

    // step 4: Create gzip files using gzopen64 and gzopen, write original data, then cleanup all resources
    gzFile gf64 = gzopen64("test_sequence_64.gz", "wb");
    gzwrite(gf64, src, (unsigned int)SRC_LEN);
    gzclose(gf64);
    gzFile gf = gzopen("test_sequence.gz", "wb");
    gzwrite(gf, src, (unsigned int)SRC_LEN);
    gzclose(gf);

    free(window);
    free(src);
    free(comp);
    free(decomp);
    free(out_desc.buf);

    // API sequence test completed successfully
    return 66;
}