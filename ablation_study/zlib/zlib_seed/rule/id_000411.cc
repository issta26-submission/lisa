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
//<ID> 411
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Helper input descriptor for inflateBack */
struct comp_desc {
    Bytef *buf;
    uLong len;
    uLong pos;
};

/* in_func for inflateBack: return pointer to next chunk of compressed data */
static unsigned in_callback(void *in_desc, unsigned char **buf) {
    struct comp_desc *d = (struct comp_desc *)in_desc;
    unsigned left = (unsigned)(d->len - d->pos);
    if (left) {
        *buf = (unsigned char *)(d->buf + d->pos);
        d->pos += left;
        return left;
    } else {
        *buf = NULL;
        return 0;
    }
}

/* out_func for inflateBack: receive decompressed data (here we discard it) */
struct out_desc {
    Bytef *buf;
    uLong pos;
};
static int out_callback(void *out_desc, unsigned char *buf, unsigned int len) {
    struct out_desc *d = (struct out_desc *)out_desc;
    /* ensure there is a storage buffer and copy the output there (or discard) */
    if (d->buf) {
        memcpy(d->buf + d->pos, buf, (size_t)len);
        d->pos += (uLong)len;
    }
    return 0;
}

int test_zlib_api_sequence() {
    /* step 1: Declarations & Initialize input and deflate stream */
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong chk = adler32(0UL, (const Bytef *)input, (uInt)inputLen);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    defStrm.adler = chk;

    /* step 2: Operate (deflate) and prepare inflate streams & header */
    deflate(&defStrm, 4); /* 4 == Z_FINISH */
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    inflateInit2_(&infStrm, 31, zlibVersion(), (int)sizeof(z_stream)); /* 15 + 16 -> 31 for gzip decoding */
    gz_header hdr;
    memset(&hdr, 0, sizeof(hdr));
    static Bytef nameBuf[32];
    memset(nameBuf, 0, sizeof(nameBuf));
    hdr.name = nameBuf;
    hdr.name_max = (uInt)sizeof(nameBuf);
    inflateGetHeader(&infStrm, &hdr);

    /* step 3: Copy inflate state and attempt back inflate using descriptors */
    z_stream copyStrm;
    memset(&copyStrm, 0, sizeof(copyStrm));
    inflateCopy(&copyStrm, &infStrm);
    struct comp_desc cdesc;
    cdesc.buf = compBuf;
    cdesc.len = defStrm.total_out;
    cdesc.pos = 0;
    struct out_desc od;
    od.buf = NULL; /* not storing decompressed output permanently */
    od.pos = 0;
    inflateBack(&copyStrm, (in_func)in_callback, (void *)&cdesc, (out_func)out_callback, (void *)&od);

    /* step 4: Validate-like cleanup and free resources */
    inflateEnd(&infStrm);
    inflateEnd(&copyStrm);
    deflateEnd(&defStrm);
    free(compBuf);
    return 66;
}