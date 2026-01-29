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
//<ID> 1087
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Helper descriptors used by inflateBack */
struct InDesc { const unsigned char *data; unsigned int len; unsigned int pos; };
struct OutDesc { unsigned char *dest; unsigned int len; unsigned int pos; };

/* in_func implementation for inflateBack (file-scope, not nested) */
static unsigned int in_func_impl(void *in_d, unsigned char **buf) {
    struct InDesc *d = (struct InDesc *)in_d;
    unsigned int remain = (d->pos < d->len) ? (d->len - d->pos) : 0;
    if (remain == 0) {
        return 0;
    }
    *buf = (unsigned char *)(d->data + d->pos);
    unsigned int ret = remain;
    d->pos += ret;
    return ret;
}

/* out_func implementation for inflateBack (file-scope, not nested) */
static int out_func_impl(void *out_d, unsigned char *buf, unsigned int len) {
    struct OutDesc *d = (struct OutDesc *)out_d;
    unsigned int tocopy = (d->pos + len <= d->len) ? len : (d->len - d->pos);
    if (tocopy) {
        memcpy(d->dest + d->pos, buf, tocopy);
        d->pos += tocopy;
    }
    return 0;
}

int test_zlib_api_sequence() {
    const uLong ORIG_LEN = 1024UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    /* Initialize deflate stream and compress */
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

    /* Initialize inflate stream and perform a normal inflate */
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
    inflateValidate(&istrm, 0);
    inflate(&istrm, Z_NO_FLUSH);

    /* Use inflateBack APIs with provided callbacks */
    z_stream bstrm;
    memset(&bstrm, 0, sizeof(bstrm));
    bstrm.zalloc = NULL;
    bstrm.zfree = NULL;
    bstrm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&bstrm, 15, window, ver, (int)sizeof(z_stream));
    struct InDesc in_desc;
    struct OutDesc out_desc;
    in_desc.data = comp;
    in_desc.len = (unsigned int)comp_size;
    in_desc.pos = 0;
    out_desc.dest = decomp;
    out_desc.len = (unsigned int)ORIG_LEN;
    out_desc.pos = 0;
    inflateBack(&bstrm, in_func_impl, (void *)&in_desc, out_func_impl, (void *)&out_desc);
    inflateBackEnd(&bstrm);

    /* Cleanup */
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(window);
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)comp_size;
    return 66;
}