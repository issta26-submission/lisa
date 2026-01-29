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
//<ID> 453
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers and compute CRCs
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    z_stream backStrm;
    memset(&backStrm, 0, sizeof(backStrm));
    z_stream copyStrm;
    memset(&copyStrm, 0, sizeof(copyStrm));
    unsigned char input[256];
    memset(input, 'X', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong len1 = inputLen / 2;
    uLong len2 = inputLen - len1;
    uLong crc1 = crc32(0UL, (const Bytef *)input, (uInt)len1);
    uLong crc2 = crc32(0UL, (const Bytef *)(input + len1), (uInt)len2);
    uLong combined_crc = crc32_combine64(crc1, crc2, (off64_t)len2);

    // step 2: Initialize a raw deflate stream and compress the input
    deflateInit2_(&defStrm, 6, 8, -15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    uLong bound = deflateBound(&defStrm, inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    deflate(&defStrm, 0);
    uLong compLen = defStrm.total_out;
    deflateEnd(&defStrm);

    // step 3: Initialize inflateBack and make a copy of the stream
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateCopy(&copyStrm, &backStrm);

    // step 4: Run inflateBack with simple in/out callbacks and cleanup
    struct InDesc { Bytef * buf; uLong size; uLong pos; };
    struct OutDesc { Bytef * buf; uLong size; uLong pos; };
    InDesc inDesc;
    inDesc.buf = compBuf;
    inDesc.size = compLen;
    inDesc.pos = 0;
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    OutDesc outDesc;
    outDesc.buf = outBuf;
    outDesc.size = inputLen;
    outDesc.pos = 0;
    in_func in_cb = (in_func)+[](void * d, unsigned char ** b) -> unsigned int {
        InDesc * desc = (InDesc *)d;
        *b = desc->buf + desc->pos;
        unsigned int avail = (unsigned int)(desc->size - desc->pos);
        desc->pos += avail;
        return avail;
    };
    out_func out_cb = (out_func)+[](void * d, unsigned char * b, unsigned int len) -> int {
        OutDesc * desc = (OutDesc *)d;
        memcpy(desc->buf + desc->pos, b, (size_t)len);
        desc->pos += len;
        return 0;
    };
    inflateBack(&backStrm, in_cb, &inDesc, out_cb, &outDesc);

    free(compBuf);
    free(outBuf);
    // API sequence test completed successfully
    (void)combined_crc;
    return 66;
}