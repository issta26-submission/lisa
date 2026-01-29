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
//<ID> 590
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char * ver = zlibVersion();
    uLong flags = zlibCompileFlags();
    (void)flags;
    Bytef * input = (Bytef *)malloc((size_t)128);
    memset(input, 'A', 128);
    uLong inputLen = (uLong)128;
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Setup - compress data and initialize streams
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    z_stream backStrm;
    memset(&backStrm, 0, sizeof(backStrm));
    unsigned char * window = (unsigned char *)malloc(1);
    memset(window, 0, 1);
    inflateBackInit_(&backStrm, 15, window, ver, (int)sizeof(z_stream));
    z_stream srcStrm;
    memset(&srcStrm, 0, sizeof(srcStrm));
    inflateInit_(&srcStrm, ver, (int)sizeof(z_stream));
    z_stream copyStrm;
    memset(&copyStrm, 0, sizeof(copyStrm));
    inflateCopy(&copyStrm, &srcStrm);

    // step 3: Operate - provide input to inflateBack and capture output, then write a byte via gzputc
    struct InDesc { const Bytef * data; uLong len; uLong pos; };
    struct OutDesc { Bytef * out; uLong capacity; uLong pos; };
    InDesc inDesc;
    inDesc.data = compBuf;
    inDesc.len = (uLong)compLen;
    inDesc.pos = 0;
    OutDesc outDesc;
    outDesc.capacity = inputLen * 2;
    outDesc.out = (Bytef *)malloc((size_t)outDesc.capacity);
    outDesc.pos = 0;
    in_func in_cb = [](void * d, unsigned char ** buf) -> unsigned int {
        InDesc * id = (InDesc *)d;
        *buf = (unsigned char *)(id->data + id->pos);
        unsigned int rem = (unsigned int)(id->len - id->pos);
        id->pos = id->len;
        return rem;
    };
    out_func out_cb = [](void * d, unsigned char * data, unsigned int len) -> int {
        OutDesc * od = (OutDesc *)d;
        memcpy(od->out + od->pos, data, (size_t)len);
        od->pos += (uLong)len;
        return 0;
    };
    inflateBack(&backStrm, in_cb, &inDesc, out_cb, &outDesc);
    gzFile gz = gzopen("tmp_test_zlib_api_sequence.gz", "wb");
    gzputc(gz, 'Z');
    gzclose(gz);

    // step 4: Validate & Cleanup
    inflateBackEnd(&backStrm);
    inflateEnd(&srcStrm);
    inflateEnd(&copyStrm);
    free(input);
    free(compBuf);
    free(window);
    free(outDesc.out);

    // API sequence test completed successfully
    return 66;
}