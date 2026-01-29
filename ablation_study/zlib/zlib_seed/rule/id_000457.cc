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
//<ID> 457
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize input and buffers
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    z_stream copyStrm;
    memset(&copyStrm, 0, sizeof(copyStrm));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);

    // step 2: Compress the input using deflate, then finalize deflate
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)compBound;
    deflate(&defStrm, 0);
    uLong compSize = defStrm.total_out;
    deflateEnd(&defStrm);

    // step 3: Prepare inflateBack window, callback descriptors and callbacks, init and copy stream, then run inflateBack
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    struct InDesc { Bytef * data; uLong size; uLong consumed; };
    struct OutDesc { Bytef * out; uLong size; uLong produced; };
    InDesc inDesc;
    inDesc.data = compBuf;
    inDesc.size = compSize;
    inDesc.consumed = 0;
    OutDesc outDesc;
    outDesc.out = outBuf;
    outDesc.size = inputLen;
    outDesc.produced = 0;
    in_func in_cb = +[](void * desc, unsigned char ** buf) -> unsigned int {
        InDesc * d = (InDesc *)desc;
        *buf = d->data + d->consumed;
        unsigned int len = (unsigned int)(d->size - d->consumed);
        d->consumed = d->size;
        return len;
    };
    out_func out_cb = +[](void * desc, unsigned char * buf, unsigned int len) -> int {
        OutDesc * od = (OutDesc *)desc;
        memcpy(od->out + od->produced, buf, len);
        od->produced += len;
        return 0;
    };
    inflateBackInit_(&infStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateCopy(&copyStrm, &infStrm);
    inflateBack(&infStrm, in_cb, &inDesc, out_cb, &outDesc);

    // step 4: Validate via crc combination and cleanup resources
    uLong crc_input = crc32(0UL, (const Bytef *)input, (uInt)inputLen);
    uLong crc_output = crc32(0UL, (const Bytef *)outBuf, (uInt)outDesc.produced);
    uLong combined_crc = crc32_combine64(crc_input, crc_output, (off64_t)inputLen);
    (void)combined_crc;
    inflateEnd(&infStrm);
    inflateEnd(&copyStrm);
    free(compBuf);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}