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
//<ID> 1089
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

typedef struct {
    Bytef *buf;
    uInt len;
    int used;
} InDesc;

typedef struct {
    Bytef *buf;
    uInt cap;
    uInt off;
} OutDesc;

static unsigned int my_in_func(void *in_desc, unsigned char **bufpp) {
    InDesc *d = (InDesc *)in_desc;
    if (d->used == 0) {
        *bufpp = d->buf;
        d->used = 1;
        return d->len;
    }
    *bufpp = NULL;
    return 0;
}

static int my_out_func(void *out_desc, unsigned char *data, unsigned int len) {
    OutDesc *o = (OutDesc *)out_desc;
    unsigned int avail = o->cap - o->off;
    unsigned int to_copy = (len <= avail) ? len : avail;
    if (to_copy > 0) {
        memcpy(o->buf + o->off, data, to_copy);
        o->off += to_copy;
    }
    return 0;
}

int test_zlib_api_sequence() {
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, Z_FINISH);
    uLong comp_size = dstrm.total_out;

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, Z_FINISH);
    inflateValidate(&istrm, 0);

    z_stream bstrm;
    memset(&bstrm, 0, sizeof(bstrm));
    bstrm.zalloc = NULL;
    bstrm.zfree = NULL;
    bstrm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(1 << 15);
    inflateBackInit_(&bstrm, 15, window, ver, (int)sizeof(z_stream));
    InDesc ind;
    ind.buf = comp;
    ind.len = (uInt)comp_size;
    ind.used = 0;
    OutDesc outd;
    outd.buf = decomp;
    outd.cap = (uInt)ORIG_LEN;
    outd.off = 0;
    inflateBack(&bstrm, (in_func)my_in_func, &ind, (out_func)my_out_func, &outd);
    inflateBackEnd(&bstrm);

    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gf, decomp, (unsigned int)ORIG_LEN);
    gzclose(gf);

    free(window);
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)comp_size;
    return 66;
}