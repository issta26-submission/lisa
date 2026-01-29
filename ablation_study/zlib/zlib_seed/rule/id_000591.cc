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
//<ID> 591
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char * ver = zlibVersion();
    Bytef * input = (Bytef *)malloc((size_t)128);
    memset(input, 'A', 128);
    uLong inputLen = (uLong)128;
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    uLong flags = zlibCompileFlags();
    (void)flags;

    // step 2: Initialize streams and descriptors
    z_stream backStrm;
    memset(&backStrm, 0, sizeof(backStrm));
    z_stream copyStrm;
    memset(&copyStrm, 0, sizeof(copyStrm));
    unsigned char * window = (unsigned char *)malloc(1);
    memset(window, 0, 1);

    struct InDesc { Bytef *data; uLong len; uLong pos; };
    struct OutDesc { Bytef *out; uLong capacity; uLong pos; };
    InDesc inDesc;
    inDesc.data = compBuf;
    inDesc.len = (uLong)compLen;
    inDesc.pos = 0;
    OutDesc outDesc;
    outDesc.capacity = inputLen;
    outDesc.out = (Bytef *)malloc((size_t)outDesc.capacity);
    memset(outDesc.out, 0, (size_t)outDesc.capacity);
    outDesc.pos = 0;

    inflateBackInit_(&backStrm, 15, window, ver, (int)sizeof(z_stream));

    // step 3: Operate: copy stream, perform back-inflation, and write via gzputc
    inflateCopy(&copyStrm, &backStrm);

    unsigned int (*in_fn)(void *, unsigned char **) = +[](void * desc, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)desc;
        if (d->pos >= d->len) { *buf = NULL; return 0U; }
        *buf = (unsigned char *)(d->data + d->pos);
        unsigned int avail = (unsigned int)(d->len - d->pos);
        d->pos += avail;
        return avail;
    };
    int (*out_fn)(void *, unsigned char *, unsigned int) = +[](void * desc, unsigned char *buf, unsigned int len) -> int {
        OutDesc *o = (OutDesc *)desc;
        unsigned int tocopy = len;
        if (o->pos + (uLong)tocopy > o->capacity) {
            tocopy = (unsigned int)(o->capacity - o->pos);
        }
        memcpy(o->out + o->pos, buf, tocopy);
        o->pos += tocopy;
        return (int)tocopy;
    };

    inflateBack(&backStrm, (in_func)in_fn, &inDesc, (out_func)out_fn, &outDesc);

    gzFile gz = gzopen("tmp_test_zlib_out.gz", "wb");
    gzputc(gz, (int)outDesc.out[0]);

    // step 4: Cleanup
    inflateBackEnd(&backStrm);
    inflateEnd(&copyStrm);
    gzclose(gz);
    free(input);
    free(compBuf);
    free(outDesc.out);
    free(window);

    // API sequence test completed successfully
    return 66;
}