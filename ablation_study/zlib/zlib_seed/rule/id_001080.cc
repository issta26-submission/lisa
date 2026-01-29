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
//<ID> 1080
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Helper input state for inflateBack in_func callback */
struct in_state {
    const unsigned char *buf;
    uLong len;
};

/* Helper output state for inflateBack out_func callback */
struct out_state {
    unsigned char *buf;
    uLong len;
};

/* in_func: provide up to 1024 bytes from the input buffer each call */
static unsigned my_in(void *in_desc, unsigned char **buf) {
    struct in_state *s = (struct in_state *)in_desc;
    if (s->len == 0) {
        *buf = NULL;
        return 0;
    }
    unsigned give = (s->len > 1024u) ? 1024u : (unsigned)s->len;
    *buf = (unsigned char *)s->buf;
    s->buf += give;
    s->len -= give;
    return give;
}

/* out_func: copy produced bytes into the output buffer */
static int my_out(void *out_desc, unsigned char *buf, unsigned len) {
    struct out_state *s = (struct out_state *)out_desc;
    if ((uLong)len > s->len) {
        /* Not enough space: truncate to available space */
        len = (unsigned)s->len;
    }
    memcpy(s->buf, buf, len);
    s->buf += len;
    s->len -= len;
    return 0; /* success */
}

int test_zlib_api_sequence() {
    /* step 1: Declarations & prepare buffers */
    const uLong ORIG_LEN = 256UL;
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
    dstrm.zalloc = Z_NULL;
    dstrm.zfree = Z_NULL;
    dstrm.opaque = Z_NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, Z_FINISH);
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    /* step 3: Initialize inflate, validate, inflate the compressed data */
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = Z_NULL;
    istrm.zfree = Z_NULL;
    istrm.opaque = Z_NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflateValidate(&istrm, 0);
    inflate(&istrm, Z_NO_FLUSH);
    inflateEnd(&istrm);

    /* step 4: Exercise inflateBack APIs with a window and cleanup */
    z_stream bstrm;
    memset(&bstrm, 0, sizeof(bstrm));
    bstrm.zalloc = Z_NULL;
    bstrm.zfree = Z_NULL;
    bstrm.opaque = Z_NULL;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&bstrm, 15, window, ver, (int)sizeof(z_stream));

    struct in_state istate;
    istate.buf = comp;
    istate.len = comp_size;
    struct out_state ostate;
    ostate.buf = decomp;
    ostate.len = ORIG_LEN;

    inflateBack(&bstrm, (in_func)my_in, (void *)&istate, (out_func)my_out, (void *)&ostate);
    inflateBackEnd(&bstrm);
    free(window);

    free(orig);
    free(comp);
    free(decomp);
    (void)comp_size;
    (void)ver;
    /* API sequence test completed successfully */
    return 66;
}