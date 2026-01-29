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
//<ID> 451
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // step 1: Declarations & prepare input and compress with deflate
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    unsigned char input[256];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)compBound;
    deflate(&defStrm, 0);
    uLong compressedLen = defStrm.total_out;

    // step 2: Initialize inflate-back stream with a window
    z_stream infBackStrm;
    memset(&infBackStrm, 0, sizeof(infBackStrm));
    const int windowBits = 15;
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&infBackStrm, windowBits, window, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Copy inflate stream, then run inflateBack with in/out callbacks and combine CRCs
    z_stream infCopy;
    memset(&infCopy, 0, sizeof(infCopy));
    inflateCopy(&infCopy, &infBackStrm);

    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen + 16);
    memset(outBuf, 0, (size_t)inputLen + 16);

    struct InDesc { const Bytef * buf; uLong len; uLong pos; };
    struct OutDesc { Bytef * buf; uLong size; uLong pos; };
    InDesc inDesc;
    inDesc.buf = compBuf;
    inDesc.len = compressedLen;
    inDesc.pos = 0;
    OutDesc outDesc;
    outDesc.buf = outBuf;
    outDesc.size = inputLen + 16;
    outDesc.pos = 0;

    in_func in_cb = +[](void * desc, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)desc;
        if (d->pos >= d->len) {
            *buf = NULL;
            return 0u;
        }
        *buf = (unsigned char *)(d->buf + d->pos);
        unsigned int avail = (unsigned int)(d->len - d->pos);
        d->pos = d->len;
        return avail;
    };
    out_func out_cb = +[](void * desc, unsigned char *buf, unsigned int len) -> int {
        OutDesc *d = (OutDesc *)desc;
        memcpy(d->buf + d->pos, buf, (size_t)len);
        d->pos += (uLong)len;
        return (int)len;
    };

    inflateBack(&infBackStrm, in_cb, &inDesc, out_cb, &outDesc);

    uLong crc1 = crc32(0UL, (const Bytef *)input, (uInt)inputLen);
    uLong crc2 = crc32(0UL, (const Bytef *)outBuf, (uInt)outDesc.pos);
    uLong combined = crc32_combine64(crc1, crc2, (off64_t)outDesc.pos);
    (void)combined;

    // step 4: Cleanup and finalize
    deflateEnd(&defStrm);
    inflateBackEnd(&infBackStrm);
    free(compBuf);
    free(window);
    free(outBuf);
    return 66;
}