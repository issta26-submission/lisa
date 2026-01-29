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
//<ID> 1284
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source buffer and create gzip files using gzopen64 (write) and gzopen (read)
    const uLong SRC_LEN = 512UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'A', (size_t)SRC_LEN);
    gzFile gw = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, src, (unsigned int)SRC_LEN);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *readbuf = (Bytef *)malloc((size_t)SRC_LEN);
    memset(readbuf, 0, (size_t)SRC_LEN);
    gzread(gr, readbuf, (unsigned int)SRC_LEN);
    gzclose(gr);

    // step 2: Compress with compress2 and then verify decompression with uncompress2
    uLong comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    uLong comp_len = comp_bound;
    const char *ver = zlibVersion();
    compress2(comp, (uLongf *)&comp_len, src, SRC_LEN, 6);
    uLongf decomp_cap = SRC_LEN + 64;
    Bytef *decomp = (Bytef *)malloc((size_t)decomp_cap);
    memset(decomp, 0, (size_t)decomp_cap);
    uLong comp_len_for_un = comp_len;
    uncompress2(decomp, &decomp_cap, comp, &comp_len_for_un);

    // step 3: Initialize inflateBack, provide in/out callbacks that read from comp and write to a buffer, then run inflateBack
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    backstrm.zalloc = NULL;
    backstrm.zfree = NULL;
    backstrm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&backstrm, 15, window, ver, (int)sizeof(z_stream));
    struct MemState { const unsigned char *data; uLong len; uLong pos; unsigned char *out; uLong out_pos; };
    unsigned char *back_out = (unsigned char *)malloc((size_t)(SRC_LEN + 128));
    memset(back_out, 0, (size_t)(SRC_LEN + 128));
    MemState state;
    state.data = comp;
    state.len = comp_len;
    state.pos = 0;
    state.out = back_out;
    state.out_pos = 0;
    in_func inf_cb = +[](void *desc, unsigned char **buf) -> unsigned int {
        MemState *s = (MemState *)desc;
        unsigned int remaining = (unsigned int)(s->len - s->pos);
        *buf = (unsigned char *)(s->data + s->pos);
        s->pos = s->len;
        return remaining;
    };
    out_func out_cb = +[](void *desc, unsigned char *buf, unsigned int len) -> int {
        MemState *s = (MemState *)desc;
        memcpy(s->out + s->out_pos, buf, (size_t)len);
        s->out_pos += len;
        return 0;
    };
    inflateBack(&backstrm, inf_cb, &state, out_cb, &state);
    inflateBackEnd(&backstrm);

    // step 4: Cleanup all resources
    free(window);
    free(src);
    free(readbuf);
    free(comp);
    free(decomp);
    free(back_out);
    // API sequence test completed successfully
    return 66;
}