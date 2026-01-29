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
//<ID> 1286
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compress it, and write original data with gzopen64
    const uLong SRC_LEN = 512UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'A', (size_t)SRC_LEN);
    uLong comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    uLong comp_len = comp_bound;
    const char *ver = zlibVersion();
    compress2(comp, (uLongf *)&comp_len, src, SRC_LEN, 6);
    gzFile gw64 = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw64, src, (unsigned int)SRC_LEN);
    gzclose(gw64);

    // step 2: Uncompress with uncompress2 and compute an Adler32 checksum of the result
    uLong decomp_len = SRC_LEN + 128UL;
    Bytef *decomp = (Bytef *)malloc((size_t)decomp_len);
    memset(decomp, 0, (size_t)decomp_len);
    uLong source_len_for_uncompress = comp_len;
    uncompress2(decomp, (uLongf *)&decomp_len, comp, &source_len_for_uncompress);
    uLong checksum = adler32(0UL, decomp, (uInt)decomp_len);
    (void)checksum;

    // step 3: Initialize inflateBack, provide callbacks, run inflateBack, then cleanup inflateBack
    struct InDesc { const unsigned char *data; uLong len; uLong pos; };
    struct OutDesc { unsigned char *data; uLong capacity; uLong written; };
    InDesc indesc;
    indesc.data = comp;
    indesc.len = comp_len;
    indesc.pos = 0;
    OutDesc outdesc;
    outdesc.data = decomp;
    outdesc.capacity = decomp_len;
    outdesc.written = 0;
    in_func in_cb = +[](void *d, unsigned char **buf) -> unsigned int {
        InDesc *id = (InDesc *)d;
        *buf = (unsigned char *)id->data + (size_t)id->pos;
        unsigned int avail = (unsigned int)(id->len - id->pos);
        id->pos = id->len;
        return avail;
    };
    out_func out_cb = +[](void *d, unsigned char *buf, unsigned int len) -> int {
        OutDesc *od = (OutDesc *)d;
        memcpy(od->data + (size_t)od->written, buf, (size_t)len);
        od->written += (uLong)len;
        return 0;
    };
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    backstrm.zalloc = NULL;
    backstrm.zfree = NULL;
    backstrm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&backstrm, 15, window, ver, (int)sizeof(z_stream));
    inflateBack(&backstrm, in_cb, &indesc, out_cb, &outdesc);
    inflateBackEnd(&backstrm);
    free(window);

    // step 4: Re-open the gzip file with gzopen, read back, cleanup all resources
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *readbuf = (Bytef *)malloc((size_t)SRC_LEN);
    memset(readbuf, 0, (size_t)SRC_LEN);
    gzread(gr, readbuf, (unsigned int)SRC_LEN);
    gzclose(gr);
    free(readbuf);
    free(src);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}