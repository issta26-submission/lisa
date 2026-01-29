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
//<ID> 415
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* input callback for inflateBack: provide a pointer to available bytes and return count */
static unsigned my_in(void *in_desc, unsigned char **buf) {
    struct InDesc { const unsigned char *data; uInt len; uInt pos; };
    struct InDesc *d = (struct InDesc *)in_desc;
    if (d->pos < d->len) {
        *buf = (unsigned char *)(d->data + d->pos);
        unsigned available = d->len - d->pos;
        d->pos = d->len; /* hand out all remaining bytes at once */
        return available;
    }
    *buf = NULL;
    return 0;
}

/* output callback for inflateBack: consume decompressed bytes */
static int my_out(void *out_desc, unsigned char *buf, unsigned int len) {
    struct OutDesc { unsigned char *out; uInt size; uInt pos; };
    struct OutDesc *o = (struct OutDesc *)out_desc;
    unsigned int to_copy = len;
    if (o->pos + to_copy > o->size) to_copy = o->size - o->pos;
    if (to_copy) memcpy(o->out + o->pos, buf, to_copy);
    o->pos += to_copy;
    return 0; /* per zlib convention: 0 = ok */
}

int test_zlib_api_sequence() {
    /* step 1: Declarations & Initialize buffers */
    z_stream defl;
    z_stream infl_gzip;
    z_stream infl_copy;
    z_stream infl_back;
    memset(&defl, 0, sizeof(defl));
    memset(&infl_gzip, 0, sizeof(infl_gzip));
    memset(&infl_copy, 0, sizeof(infl_copy));
    memset(&infl_back, 0, sizeof(infl_back));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    unsigned char window_buf[32768];
    memset(window_buf, 0, sizeof(window_buf));
    gz_header head;
    memset(&head, 0, sizeof(head));
    unsigned char outBuf[512];
    memset(outBuf, 0, sizeof(outBuf));

    /* step 2: Initialize and configure streams */
    deflateInit_(&defl, 6, zlibVersion(), (int)sizeof(z_stream));
    defl.next_in = (Bytef *)input;
    defl.avail_in = (uInt)inputLen;
    defl.next_out = compBuf;
    defl.avail_out = (uInt)bound;
    deflate(&defl, Z_FINISH);
    uLong compLen = defl.total_out;

    inflateInit_(&infl_gzip, zlibVersion(), (int)sizeof(z_stream));
    inflateGetHeader(&infl_gzip, &head);
    inflateBackInit_(&infl_back, 15, window_buf, zlibVersion(), (int)sizeof(z_stream));

    /* step 3: Operate (copy inflate state, and run inflateBack) */
    inflateCopy(&infl_copy, &infl_gzip);

    struct InDesc { const unsigned char *data; uInt len; uInt pos; };
    struct OutDesc { unsigned char *out; uInt size; uInt pos; };
    struct InDesc in_desc;
    in_desc.data = compBuf;
    in_desc.len = (uInt)compLen;
    in_desc.pos = 0;
    struct OutDesc out_desc;
    out_desc.out = outBuf;
    out_desc.size = (uInt)sizeof(outBuf);
    out_desc.pos = 0;

    inflateBack(&infl_back, (in_func)my_in, &in_desc, (out_func)my_out, &out_desc);

    /* step 4: Validate-like cleanup and free resources */
    deflateEnd(&defl);
    inflateEnd(&infl_gzip);
    inflateBackEnd(&infl_back);
    inflateEnd(&infl_copy);
    free(compBuf);
    (void)out_desc.pos; /* use to avoid unused warning */

    return 66; /* API sequence test completed successfully */
}