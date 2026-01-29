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
//<ID> 763
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Helper input descriptor for inflateBack in_func */
typedef struct {
    const unsigned char *ptr;
    unsigned int remaining;
} in_desc_t;

/* Helper output descriptor for inflateBack out_func */
typedef struct {
    unsigned char *ptr;
    unsigned int remaining;
    unsigned int written;
} out_desc_t;

/* in_func implementation: provide a pointer to available input bytes */
static unsigned in_callback(void *in_desc_void, unsigned char **buf) {
    in_desc_t *d = (in_desc_t *)in_desc_void;
    *buf = (unsigned char *)d->ptr;
    unsigned int n = d->remaining;
    d->ptr += n;
    d->remaining = 0;
    return n;
}

/* out_func implementation: consume output bytes into a buffer */
static int out_callback(void *out_desc_void, unsigned char *buf, unsigned int len) {
    out_desc_t *d = (out_desc_t *)out_desc_void;
    if (len > d->remaining) {
        len = d->remaining;
    }
    memcpy(d->ptr + d->written, buf, (size_t)len);
    d->written += len;
    d->remaining -= len;
    return 0;
}

int test_zlib_api_sequence() {
    /* step 1: Prepare input data and compress it */
    const uLong INPUT_LEN = 512;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'Z', (size_t)INPUT_LEN);
    uLong bound = compressBound(INPUT_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress(comp, &compLen, input, INPUT_LEN);

    /* step 2: Initialize inflate stream and retrieve gzip header structure */
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inf.zalloc = Z_NULL;
    inf.zfree = Z_NULL;
    inf.opaque = Z_NULL;
    inflateInit2_(&inf, 15 + 16, zlibVersion(), (int)sizeof(z_stream)); /* enable gzip decoding */
    gz_header head;
    memset(&head, 0, sizeof(head));
    inflateGetHeader(&inf, &head);

    /* step 3: Configure dictionary and perform inflateBack operation */
    Bytef *dict = (Bytef *)malloc(32);
    memset(dict, 'D', 32);
    uInt dictLen = (uInt)32;
    inflateSetDictionary(&inf, dict, dictLen);

    uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *out = (Bytef *)malloc((size_t)OUT_LEN);
    memset(out, 0, (size_t)OUT_LEN);

    in_desc_t in_desc;
    in_desc.ptr = comp;
    in_desc.remaining = (unsigned int)compLen;

    out_desc_t out_desc;
    out_desc.ptr = out;
    out_desc.remaining = (unsigned int)OUT_LEN;
    out_desc.written = 0;

    inflateBack(&inf, (in_func)in_callback, &in_desc, (out_func)out_callback, &out_desc);

    /* step 4: Cleanup resources */
    inflateEnd(&inf);
    free(input);
    free(comp);
    free(dict);
    free(out);

    return 66;
}