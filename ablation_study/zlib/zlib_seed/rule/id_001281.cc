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
//<ID> 1281
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and produce compressed data (compress2)
    const uLong SRC_LEN = 512UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'A', (size_t)SRC_LEN);
    const char *ver = zlibVersion();
    uLong comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    uLongf comp_len_f = (uLongf)comp_bound;
    compress2(comp, &comp_len_f, src, SRC_LEN, 6);
    uLong comp_size = (uLong)comp_len_f;

    // step 2: Write original data into a gzip file using gzopen64 and close it
    gzFile gw64 = gzopen64("test_zlib_api_sequence_64.gz", "wb");
    gzwrite(gw64, src, (unsigned int)SRC_LEN);
    gzclose(gw64);

    // step 3: Decompress using uncompress2 (simple inflate path) into decomp buffer
    uLong decomp_len_ul = SRC_LEN + 128UL;
    uLongf decomp_len_f = (uLongf)decomp_len_ul;
    Bytef *decomp = (Bytef *)malloc((size_t)decomp_len_f);
    memset(decomp, 0, (size_t)decomp_len_f);
    uLong srclen_for_uncompress = comp_size;
    uncompress2(decomp, &decomp_len_f, comp, &srclen_for_uncompress);

    // step 4: Initialize inflateBack, run inflateBack with callbacks, read gzip via gzopen, then cleanup
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    backstrm.zalloc = NULL;
    backstrm.zfree = NULL;
    backstrm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);

    // Local state for callbacks
    struct InState { const unsigned char *ptr; uLong avail; };
    struct OutState { unsigned char *ptr; uLong avail; uLong written; };
    InState ist;
    ist.ptr = comp;
    ist.avail = comp_size;
    OutState ost;
    ost.ptr = (unsigned char *)malloc((size_t)(SRC_LEN + 256));
    ost.avail = SRC_LEN + 256UL;
    ost.written = 0;

    in_func in_cb = +[](void *desc, unsigned char **buf)->unsigned int {
        InState *s = (InState *)desc;
        if (s->avail == 0) return 0;
        *buf = (unsigned char *)s->ptr;
        unsigned int n = (unsigned int)s->avail;
        s->ptr += n;
        s->avail = 0;
        return n;
    };
    out_func out_cb = +[](void *desc, unsigned char *buf, unsigned int len)->int {
        OutState *o = (OutState *)desc;
        unsigned int to_copy = len;
        if ((uLong)to_copy > o->avail) to_copy = (unsigned int)o->avail;
        memcpy(o->ptr + o->written, buf, (size_t)to_copy);
        o->written += to_copy;
        if ((uLong)to_copy < len) return 1;
        return 0;
    };

    inflateBackInit_(&backstrm, 15, window, ver, (int)sizeof(z_stream));
    inflateBack(&backstrm, in_cb, &ist, out_cb, &ost);
    inflateBackEnd(&backstrm);

    gzFile gr = gzopen("test_zlib_api_sequence_64.gz", "rb");
    Bytef *readbuf = (Bytef *)malloc((size_t)SRC_LEN);
    memset(readbuf, 0, (size_t)SRC_LEN);
    gzread(gr, readbuf, (unsigned int)SRC_LEN);
    gzclose(gr);

    free(ost.ptr);
    free(window);
    free(src);
    free(comp);
    free(decomp);
    free(readbuf);

    // API sequence test completed successfully
    return 66;
}