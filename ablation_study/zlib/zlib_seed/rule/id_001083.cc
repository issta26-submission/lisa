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
//<ID> 1083
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Helper descriptors and callbacks for inflateBack (allowed outside the main test function). */
typedef struct {
    const unsigned char *data;
    unsigned int len;
    unsigned int pos;
} InDesc;

typedef struct {
    unsigned char *out;
    unsigned int capacity;
    unsigned int pos;
} OutDesc;

/* in_func: supplies compressed input to inflateBack.
   Signature matches: unsigned (*in_func)(void *, unsigned char **)
*/
static unsigned in_callback(void *in_desc, unsigned char **buf) {
    InDesc *d = (InDesc *)in_desc;
    if (d->pos >= d->len) {
        return 0;
    }
    *buf = (unsigned char *)(d->data + d->pos);
    unsigned avail = d->len - d->pos;
    d->pos = d->len; /* supply all remaining at once */
    return avail;
}

/* out_func: receives decompressed output from inflateBack.
   Signature matches: int (*out_func)(void *, unsigned char *, unsigned int)
*/
static int out_callback(void *out_desc, unsigned char *buf, unsigned int len) {
    OutDesc *d = (OutDesc *)out_desc;
    if (len > (d->capacity - d->pos)) {
        /* indicate error by returning non-zero (handled by inflateBack). */
        return 1;
    }
    memcpy(d->out + d->pos, buf, len);
    d->pos += len;
    return 0;
}

int test_zlib_api_sequence() {
    /* step 1: Declarations & prepare buffers */
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    /* step 2: Initialize deflate, compress data, and finalize deflate */
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
    deflateEnd(&dstrm);

    /* step 3: Initialize inflate, perform inflate, validate, exercise inflateBack */
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
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&bstrm, 15, window, ver, (int)sizeof(z_stream));

    InDesc inDesc;
    inDesc.data = comp;
    inDesc.len = (unsigned int)comp_size;
    inDesc.pos = 0;

    OutDesc outDesc;
    outDesc.out = decomp;
    outDesc.capacity = (unsigned int)ORIG_LEN;
    outDesc.pos = 0;

    inflateBack(&bstrm, (in_func)in_callback, (void *)&inDesc, (out_func)out_callback, (void *)&outDesc);
    inflateBackEnd(&bstrm);
    free(window);

    /* step 4: Cleanup and finalize */
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);

    /* API sequence test completed successfully */
    return 66;
}