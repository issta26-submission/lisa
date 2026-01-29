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
//<ID> 455
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream backSrc;
    memset(&backSrc, 0, sizeof(backSrc));
    z_stream backDst;
    memset(&backDst, 0, sizeof(backDst));
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Compress input and perform a simple deflate init/end
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateEnd(&defStrm);

    // step 3: Initialize inflate-back, copy stream state, run inflateBack with callbacks, combine CRCs
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&backSrc, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateCopy(&backDst, &backSrc);

    struct InState { const unsigned char * data; uLong remaining; uLong offset; };
    struct OutState { unsigned char * out; uLong written; uLong capacity; };

    InState in_state;
    in_state.data = (const unsigned char *)compBuf;
    in_state.remaining = (uLong)compLen;
    in_state.offset = 0;

    unsigned char * outBuf = (unsigned char *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    OutState out_state;
    out_state.out = outBuf;
    out_state.written = 0;
    out_state.capacity = inputLen;

    unsigned int (*in_cb)(void *, unsigned char **) = [](void * in_desc, unsigned char ** buf) -> unsigned int {
        InState * s = (InState *)in_desc;
        unsigned int avail = (unsigned int)s->remaining;
        *buf = (unsigned char *)(s->data + s->offset);
        s->offset += (uLong)avail;
        s->remaining = 0;
        return avail;
    };
    int (*out_cb)(void *, unsigned char *, unsigned int) = [](void * out_desc, unsigned char * data, unsigned int len) -> int {
        OutState * s = (OutState *)out_desc;
        memcpy(s->out + s->written, data, (size_t)len);
        s->written += (uLong)len;
        return 0;
    };

    inflateBack(&backDst, in_cb, (void *)&in_state, out_cb, (void *)&out_state);

    uLong crcA = crc32(0UL, (const Bytef *)input, (uInt)(inputLen / 2));
    uLong crcB = crc32(0UL, (const Bytef *)(input + (inputLen / 2)), (uInt)(inputLen - (inputLen / 2)));
    uLong combined_crc = crc32_combine64(crcA, crcB, (off64_t)(inputLen - (inputLen / 2)));
    (void)combined_crc;

    // step 4: Cleanup and finalize
    inflateBackEnd(&backDst);
    inflateBackEnd(&backSrc);
    inflateEnd(&backDst);
    inflateEnd(&backSrc);
    free(compBuf);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}