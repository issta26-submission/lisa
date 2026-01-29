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
//<ID> 595
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Helper state structures for inflateBack in/out functions */
struct in_state {
    const unsigned char *ptr;
    unsigned int len;
};
struct out_state {
    unsigned char *ptr;
    unsigned int capacity;
    unsigned int written;
};

/* in_func for inflateBack: returns available bytes and sets *buf to point at them */
static unsigned in_func_stub(void *in_desc, unsigned char **buf) {
    struct in_state *s = (struct in_state *)in_desc;
    if (s->len == 0) {
        *buf = NULL;
        return 0;
    }
    *buf = (unsigned char *)s->ptr;
    unsigned avail = s->len;
    s->ptr += avail;
    s->len = 0;
    return avail;
}

/* out_func for inflateBack: writes data to provided out_state buffer */
static int out_func_stub(void *out_desc, unsigned char *buf, unsigned int len) {
    struct out_state *o = (struct out_state *)out_desc;
    unsigned int space = o->capacity - o->written;
    unsigned int tocopy = (len <= space) ? len : space;
    if (tocopy > 0) {
        memcpy(o->ptr + o->written, buf, tocopy);
        o->written += tocopy;
    }
    return 0; /* return 0 on success */
}

int test_zlib_api_sequence() {
    /* step 1: Declarations & prepare buffers */
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    z_stream infSrc;
    memset(&infSrc, 0, sizeof(infSrc));
    z_stream infCopy;
    memset(&infCopy, 0, sizeof(infCopy));
    z_stream infBack;
    memset(&infBack, 0, sizeof(infBack));

    const char * ver = zlibVersion();
    Bytef * input = (Bytef *)malloc((size_t)128);
    memset(input, 'A', 128);
    uLong inputLen = (uLong)128;
    Bytef * compBuf = NULL;
    Bytef * outBuf = NULL;
    Bytef * outBuf2 = NULL;
    unsigned char * windowBuf = NULL;

    /* step 2: Initialize compressor, compute bound, allocate buffers, and initialize inflater(s) */
    deflateInit_(&defStrm, 6, ver, (int)sizeof(z_stream));
    uLong compBound = deflateBound(&defStrm, inputLen);
    compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    defStrm.next_in = input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)compBound;

    /* Initialize a normal inflate stream (usable with inflateCopy) */
    inflateInit_(&infSrc, ver, (int)sizeof(z_stream));
    outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    infSrc.next_out = outBuf;
    infSrc.avail_out = (uInt)inputLen;

    /* Initialize a stream for inflateBack (requires a window buffer) */
    windowBuf = (unsigned char *)malloc(1u << 15); /* 32KB window */
    inflateBackInit_(&infBack, 15, windowBuf, ver, (int)sizeof(z_stream));

    /* step 3: Operate: compress, prepare inflate input, copy inflater, call inflateBack, query flags and write gz file */
    deflate(&defStrm, Z_FINISH);
    infSrc.next_in = compBuf;
    infSrc.avail_in = (uInt)defStrm.total_out;
    inflateCopy(&infCopy, &infSrc);

    /* Prepare out buffer for inflateBack and in_state describing compressed data */
    outBuf2 = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf2, 0, (size_t)inputLen);
    struct in_state ist;
    ist.ptr = compBuf;
    ist.len = (unsigned int)defStrm.total_out;
    struct out_state ost;
    ost.ptr = outBuf2;
    ost.capacity = (unsigned int)inputLen;
    ost.written = 0;

    inflateBack(&infBack, in_func_stub, &ist, out_func_stub, &ost);
    inflateBackEnd(&infBack);

    uLong flags = zlibCompileFlags();
    (void)flags;
    gzFile gz = gzopen("tmp_zlib_api.gz", "wb");
    gzputc(gz, 'Z');
    gzclose(gz);

    /* step 4: Cleanup */
    deflateEnd(&defStrm);
    inflateEnd(&infSrc);
    inflateEnd(&infCopy);
    inflateEnd(&infBack);
    free(input);
    free(compBuf);
    free(outBuf);
    free(outBuf2);
    free(windowBuf);
    return 66;
}