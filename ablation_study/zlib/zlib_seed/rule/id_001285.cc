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
//<ID> 1285
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Helper descriptors and callbacks for inflateBack */
typedef struct {
    unsigned char *ptr;
    unsigned int len;
} in_desc_t;

typedef struct {
    unsigned char *ptr;
    unsigned int len;
} out_desc_t;

static unsigned my_in(void *in_desc_void, unsigned char **buf) {
    in_desc_t *d = (in_desc_t *)in_desc_void;
    if (d->len == 0) {
        *buf = NULL;
        return 0;
    }
    *buf = d->ptr;
    unsigned avail = d->len;
    d->ptr += avail;
    d->len = 0;
    return avail;
}

static int my_out(void *out_desc_void, unsigned char *buf, unsigned int len) {
    out_desc_t *d = (out_desc_t *)out_desc_void;
    if (len > d->len) {
        len = d->len;
    }
    memcpy(d->ptr, buf, len);
    d->ptr += len;
    d->len -= len;
    return 0;
}

int test_zlib_api_sequence() {
    const uLong SRC_LEN = 512UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'A', (size_t)SRC_LEN);

    uLong comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    const char *ver = zlibVersion();
    compress2(comp, &comp_len, src, SRC_LEN, 6);

    gzFile gf1 = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gf1, src, (unsigned int)SRC_LEN);
    gzclose(gf1);

    gzFile gf2 = gzopen64("test_zlib_api_sequence64.gz", "wb");
    gzwrite(gf2, src, (unsigned int)SRC_LEN);
    gzclose(gf2);

    uLong decomp_len = SRC_LEN + 64;
    Bytef *decomp = (Bytef *)malloc((size_t)decomp_len);
    uLongf destLen = (uLongf)decomp_len;
    uLong sourceLen = (uLong)comp_len;
    uncompress2(decomp, &destLen, comp, &sourceLen);

    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    backstrm.zalloc = NULL;
    backstrm.zfree = NULL;
    backstrm.opaque = NULL;

    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);

    inflateBackInit_(&backstrm, 15, window, ver, (int)sizeof(z_stream));

    in_desc_t in_desc;
    in_desc.ptr = comp;
    /* comp_len may be larger than unsigned int; cap to UINT_MAX where appropriate */
    if (comp_len > (uLong)0xFFFFFFFFUL) {
        in_desc.len = 0xFFFFFFFFU;
    } else {
        in_desc.len = (unsigned int)comp_len;
    }

    out_desc_t out_desc;
    out_desc.ptr = (unsigned char *)malloc((size_t)(SRC_LEN + 256));
    out_desc.len = (unsigned int)(SRC_LEN + 256);

    inflateBack(&backstrm, (in_func)my_in, (void *)&in_desc, (out_func)my_out, (void *)&out_desc);

    inflateBackEnd(&backstrm);

    free(out_desc.ptr);
    free(src);
    free(comp);
    free(decomp);
    free(window);
    return 66;
}