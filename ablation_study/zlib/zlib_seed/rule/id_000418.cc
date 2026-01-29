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
//<ID> 418
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Simple input descriptor for inflateBack in_func */
typedef struct {
    unsigned char *buf;
    size_t len;
    size_t pos;
} in_desc_t;

/* Simple output descriptor for inflateBack out_func */
typedef struct {
    unsigned char *buf;
    size_t len;
    size_t pos;
} out_desc_t;

/* in_func for inflateBack: provide pointer to next available input bytes */
static unsigned int fb_in_func(void *in_desc_v, unsigned char **bufptr) {
    in_desc_t *d = (in_desc_t *)in_desc_v;
    if (d->pos >= d->len) {
        return 0;
    }
    *bufptr = d->buf + d->pos;
    /* provide all remaining bytes once */
    unsigned int avail = (unsigned int)(d->len - d->pos);
    d->pos += avail;
    return avail;
}

/* out_func for inflateBack: consume output bytes (store into provided buffer in descriptor) */
static int fb_out_func(void *out_desc_v, unsigned char *buf, unsigned int len) {
    out_desc_t *d = (out_desc_t *)out_desc_v;
    /* copy output into descriptor buffer if space, otherwise truncate */
    unsigned int copy_len = len;
    if (d->pos + copy_len > d->len) {
        copy_len = (unsigned int)(d->len - d->pos);
    }
    if (copy_len > 0) {
        memcpy(d->buf + d->pos, buf, copy_len);
        d->pos += copy_len;
    }
    return 0; /* indicate success */
}

int test_zlib_api_sequence() {
    /* step 1: Declarations & Initialize buffers and streams */
    z_stream defStrm;
    z_stream infStrm;
    z_stream copyStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    memset(&infStrm, 0, sizeof(infStrm));
    memset(&copyStrm, 0, sizeof(copyStrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);

    /* Initialize deflate for gzip output (windowBits = 15 + 16 -> gzip header) */
    deflateInit2_(&defStrm, 6, 8, 15 + 16, 8, 0, zlibVersion(), (int)sizeof(z_stream));

    /* step 2: Configure deflate stream and perform deflate (finish the stream) */
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    deflate(&defStrm, 4); /* 4 == Z_FINISH */

    /* step 3: Initialize inflate stream to accept gzip, copy it, and retrieve gzip header info */
    inflateInit2_(&infStrm, 15 + 16, zlibVersion(), (int)sizeof(z_stream));
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)defStrm.total_out;
    /* prepare an output buffer for inflate (not used for inflateBack) */
    unsigned char outBuf[128];
    memset(outBuf, 0, sizeof(outBuf));
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)sizeof(outBuf);
    inflateCopy(&copyStrm, &infStrm);
    gz_header head;
    memset(&head, 0, sizeof(head));
    inflateGetHeader(&copyStrm, &head);

    /* step 4: Prepare inflateBack: reinitialize copyStrm for inflateBack usage */
    inflateEnd(&copyStrm);
    unsigned char *windowBuf = (unsigned char *)malloc(1 << 15); /* window for inflateBack */
    memset(windowBuf, 0, 1 << 15);
    inflateBackInit_(&copyStrm, 15, windowBuf, zlibVersion(), (int)sizeof(z_stream));

    /* Prepare descriptors for inflateBack callbacks */
    in_desc_t in_desc;
    in_desc.buf = compBuf;
    in_desc.len = (size_t)defStrm.total_out;
    in_desc.pos = 0;
    out_desc_t out_desc;
    out_desc.buf = outBuf;
    out_desc.len = sizeof(outBuf);
    out_desc.pos = 0;

    /* Call inflateBack with the callbacks */
    inflateBack(&copyStrm, (in_func)fb_in_func, &in_desc, (out_func)fb_out_func, &out_desc);

    /* Cleanup */
    inflateBackEnd(&copyStrm);
    inflateEnd(&infStrm);
    deflateEnd(&defStrm);
    free(windowBuf);
    free(compBuf);
    return 66;
}