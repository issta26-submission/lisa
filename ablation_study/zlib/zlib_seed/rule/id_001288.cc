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
//<ID> 1288
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compress with compress2
    const uLong SRC_LEN = 512UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'A', (size_t)SRC_LEN);
    uLongf comp_cap = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_cap);
    memset(comp, 0, (size_t)comp_cap);
    uLong comp_len = (uLong)comp_cap;
    compress2(comp, &comp_len, src, SRC_LEN, 6);

    // step 2: Use uncompress2 to decompress into a buffer
    uLongf decomp1_cap = SRC_LEN + 64;
    Bytef *decomp1 = (Bytef *)malloc((size_t)decomp1_cap);
    memset(decomp1, 0, (size_t)decomp1_cap);
    uLong comp_len_for_uncomp = comp_len;
    uncompress2(decomp1, &decomp1_cap, comp, &comp_len_for_uncomp);

    // step 3: Initialize inflateBack stream and run inflateBack with callbacks
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    backstrm.zalloc = NULL;
    backstrm.zfree = NULL;
    backstrm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    const char *ver = zlibVersion();
    inflateBackInit_(&backstrm, 15, window, ver, (int)sizeof(z_stream));

    struct InDesc { Bytef *buf; uLong len; uLong pos; };
    struct OutDesc { Bytef *buf; uLong cap; uLong pos; };
    InDesc indesc;
    indesc.buf = comp;
    indesc.len = comp_len;
    indesc.pos = 0;
    uLongf decomp2_cap = SRC_LEN + 128;
    Bytef *decomp2 = (Bytef *)malloc((size_t)decomp2_cap);
    memset(decomp2, 0, (size_t)decomp2_cap);
    OutDesc outdesc;
    outdesc.buf = decomp2;
    outdesc.cap = decomp2_cap;
    outdesc.pos = 0;

    in_func in_cb = [](void *in_d, unsigned char **buf) -> unsigned int {
        InDesc *id = (InDesc *)in_d;
        if (id->pos >= id->len) {
            *buf = NULL;
            return 0;
        }
        *buf = id->buf + id->pos;
        unsigned int avail = (unsigned int)(id->len - id->pos);
        id->pos += avail;
        return avail;
    };
    out_func out_cb = [](void *out_d, unsigned char *buf, unsigned int len) -> int {
        OutDesc *od = (OutDesc *)out_d;
        memcpy(od->buf + od->pos, buf, (size_t)len);
        od->pos += len;
        return 0;
    };

    inflateBack(&backstrm, in_cb, &indesc, out_cb, &outdesc);
    inflateBackEnd(&backstrm);
    free(window);

    // step 4: Write original data to gzip via gzopen64 and read it back via gzopen
    gzFile gw64 = gzopen64("test_zlib_api_sequence_64.gz", "wb");
    gzwrite(gw64, src, (unsigned int)SRC_LEN);
    gzclose(gw64);
    gzFile gr = gzopen("test_zlib_api_sequence_64.gz", "rb");
    Bytef *readbuf = (Bytef *)malloc((size_t)SRC_LEN);
    memset(readbuf, 0, (size_t)SRC_LEN);
    gzread(gr, readbuf, (unsigned int)SRC_LEN);
    gzclose(gr);

    // cleanup
    free(src);
    free(comp);
    free(decomp1);
    free(decomp2);
    free(readbuf);

    // API sequence test completed successfully
    return 66;
}