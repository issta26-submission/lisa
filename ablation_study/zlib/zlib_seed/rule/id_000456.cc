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
//<ID> 456
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers and streams
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    z_stream backStrm;
    memset(&backStrm, 0, sizeof(backStrm));
    z_stream copyStrm;
    memset(&copyStrm, 0, sizeof(copyStrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;

    // step 2: Initialize deflate, compress the input into compBuf
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflate(&defStrm, 0);
    uLong compLen = defStrm.total_out;

    // step 3: Initialize inflateBack, prepare descriptors and run inflateBack
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    struct Desc { unsigned char * data; unsigned int len; unsigned int pos; unsigned char * out; unsigned int outlen; unsigned int outpos; };
    Desc desc;
    desc.data = (unsigned char *)compBuf;
    desc.len = (unsigned int)compLen;
    desc.pos = 0;
    desc.outlen = (unsigned int)inputLen;
    desc.outpos = 0;
    desc.out = (unsigned char *)malloc((size_t)desc.outlen);
    memset(desc.out, 0, (size_t)desc.outlen);
    auto in_lambda = +[](void * d, unsigned char ** buf) -> unsigned int {
        Desc * dd = (Desc *)d;
        unsigned int rem = dd->len - dd->pos;
        *buf = dd->data + dd->pos;
        dd->pos = dd->len;
        return rem;
    };
    auto out_lambda = +[](void * d, unsigned char * buf, unsigned int len) -> int {
        Desc * dd = (Desc *)d;
        memcpy(dd->out + dd->outpos, buf, (size_t)len);
        dd->outpos += len;
        return 0;
    };
    inflateBack(&backStrm,
                (unsigned int (*)(void *, unsigned char **))in_lambda,
                &desc,
                (int (*)(void *, unsigned char *, unsigned int))out_lambda,
                &desc);

    // step 4: Copy inflate state, compute checksums, cleanup
    inflateCopy(&copyStrm, &backStrm);
    uLong crc1 = crc32(0UL, (const Bytef *)input, (uInt)inputLen);
    uLong crc2 = crc32(0UL, (const Bytef *)desc.out, (uInt)desc.outpos);
    uLong combined = crc32_combine64(crc1, crc2, (off64_t)desc.outpos);
    (void)combined;
    deflateEnd(&defStrm);
    inflateEnd(&backStrm);
    inflateEnd(&copyStrm);
    free(compBuf);
    free(desc.out);
    return 66;
    // API sequence test completed successfully
}