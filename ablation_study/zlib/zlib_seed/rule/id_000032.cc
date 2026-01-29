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
//<ID> 32
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

typedef struct {
    unsigned char *buf;
    uLong len;
    uLong pos;
} in_ctx_t;

typedef struct {
    unsigned char *buf;
    uLong len;
    uLong pos;
} out_ctx_t;

/* in_func: supply compressed bytes to inflateBack */
static unsigned in_cb(void *desc, unsigned char **bufp) {
    in_ctx_t *c = (in_ctx_t *)desc;
    *bufp = c->buf + c->pos;
    unsigned remaining = (unsigned)(c->len - c->pos);
    c->pos += remaining;
    return remaining;
}

/* out_func: receive decompressed bytes from inflateBack */
static int out_cb(void *desc, unsigned char *buf, unsigned int len) {
    out_ctx_t *o = (out_ctx_t *)desc;
    unsigned int copy_len = len;
    if ((uLong)copy_len > (o->len - o->pos)) {
        copy_len = (unsigned int)(o->len - o->pos);
    }
    memcpy(o->buf + o->pos, buf, copy_len);
    o->pos += copy_len;
    return 0;
}

int test_zlib_api_sequence() {
    /* step 1: Declarations & Initialize */
    z_stream dstrm;
    z_stream istrm;
    z_stream istrm_copy;
    z_stream istrm_back;
    unsigned char input[256];
    unsigned char decomp[256];
    unsigned char decomp_back[256];
    gzFile gz = 0;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    memset(&istrm_back, 0, sizeof(istrm_back));
    memset(input, 'A', sizeof(input));
    memset(decomp, 0, sizeof(decomp));
    memset(decomp_back, 0, sizeof(decomp_back));
    uLong sourceLen = (uLong)sizeof(input);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cb);

    /* step 2: Setup and initialize compressors/inflaters */
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)cb;
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflate(&dstrm, Z_FINISH);
    deflateEnd(&dstrm);

    istrm.next_in = comp;
    istrm.avail_in = (uInt)dstrm.total_out;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)sizeof(decomp);
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    inflate(&istrm, Z_FINISH);

    /* make a copy of the inflate stream state */
    inflateCopy(&istrm_copy, &istrm);

    /* initialize a separate stream for inflateBack and run inflateBack with proper callbacks */
    unsigned char *window = (unsigned char *)malloc((1u << 15));
    in_ctx_t ic;
    out_ctx_t oc;
    ic.buf = comp;
    ic.len = (uLong)dstrm.total_out;
    ic.pos = 0;
    oc.buf = decomp_back;
    oc.len = (uLong)sizeof(decomp_back);
    oc.pos = 0;
    inflateBackInit_(&istrm_back, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateBack(&istrm_back, (in_func)in_cb, (void *)&ic, (out_func)out_cb, (void *)&oc);
    inflateBackEnd(&istrm_back);
    free(window);

    /* write some decompressed bytes to a gz file */
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, decomp, (unsigned int)2);
    gzflush(gz, Z_FINISH);

    /* step 4: Cleanup */
    inflateEnd(&istrm);
    inflateEnd(&istrm_copy);
    gzclose(gz);
    free(comp);

    return 66;
}