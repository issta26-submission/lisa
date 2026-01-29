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
//<ID> 1282
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, zlib-compress to zlib-wrapped format, then uncompress2 to validate round-trip
    const uLong SRC_LEN = 512UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'A', (size_t)SRC_LEN);
    uLongf comp_zlib_bound = compressBound(SRC_LEN);
    Bytef *comp_zlib = (Bytef *)malloc((size_t)comp_zlib_bound);
    uLongf comp_zlib_len = comp_zlib_bound;
    const char *ver = zlibVersion();
    compress2(comp_zlib, &comp_zlib_len, src, SRC_LEN, 6);
    uLong source_len_for_uncomp = (uLong)comp_zlib_len;
    uLongf decomp_buf_len = SRC_LEN + 64;
    Bytef *decomp = (Bytef *)malloc((size_t)decomp_buf_len);
    uncompress2(decomp, &decomp_buf_len, comp_zlib, &source_len_for_uncomp);

    // step 2: Create a raw deflate stream from src using deflateInit2_, producing comp_raw
    uLongf comp_raw_bound = compressBound(SRC_LEN);
    Bytef *comp_raw = (Bytef *)malloc((size_t)comp_raw_bound);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit2_(&dstrm, 6, 8, -15, 8, 0, ver, (int)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)SRC_LEN;
    dstrm.next_out = comp_raw;
    dstrm.avail_out = (uInt)comp_raw_bound;
    deflate(&dstrm, 4);
    uLong comp_raw_len = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflateBack, provide in/out callbacks, run inflateBack and cleanup the inflateBack state
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    backstrm.zalloc = NULL;
    backstrm.zfree = NULL;
    backstrm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&backstrm, -15, window, ver, (int)sizeof(z_stream));
    struct InDesc { const unsigned char *data; uLong len; uLong pos; };
    struct OutDesc { unsigned char *out; uLong cap; uLong pos; };
    InDesc in_desc = { comp_raw, comp_raw_len, 0 };
    OutDesc out_desc;
    out_desc.cap = SRC_LEN + 128;
    out_desc.out = (unsigned char *)malloc((size_t)out_desc.cap);
    out_desc.pos = 0;
    in_func in_cb = [](void *d, unsigned char **buf) -> unsigned int {
        InDesc *id = (InDesc *)d;
        *buf = (unsigned char *)(id->data + id->pos);
        unsigned int ret = (unsigned int)(id->len - id->pos);
        id->pos = id->len;
        return ret;
    };
    out_func out_cb = [](void *d, unsigned char *buf, unsigned int len) -> int {
        OutDesc *od = (OutDesc *)d;
        memcpy(od->out + od->pos, buf, (size_t)len);
        od->pos += (uLong)len;
        return 0;
    };
    inflateBack(&backstrm, in_cb, &in_desc, out_cb, &out_desc);
    inflateBackEnd(&backstrm);

    // step 4: Use gzopen64 and gzopen to write original data to gzip files, then cleanup all resources
    gzFile g64 = gzopen64("test_zlib_sequence_64.gz", "wb");
    gzwrite(g64, src, (unsigned int)SRC_LEN);
    gzclose(g64);
    gzFile g = gzopen("test_zlib_sequence.gz", "wb");
    gzwrite(g, src, (unsigned int)SRC_LEN);
    gzclose(g);
    free(src);
    free(comp_zlib);
    free(decomp);
    free(comp_raw);
    free(window);
    free(out_desc.out);
    // API sequence test completed successfully
    return 66;
}