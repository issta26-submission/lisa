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
//<ID> 593
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare data
    const char * ver = zlibVersion();
    uLong flags = zlibCompileFlags();
    (void)flags;
    Bytef * input = (Bytef *)malloc((size_t)128);
    memset(input, 'A', 128);
    uLong inputLen = (uLong)128;
    uLongf compBufLen = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)compBufLen);
    memset(compBuf, 0, (size_t)compBufLen);
    compress(compBuf, &compBufLen, input, inputLen);
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    z_stream copyStrm;
    memset(&copyStrm, 0, sizeof(copyStrm));
    z_stream backStrm;
    memset(&backStrm, 0, sizeof(backStrm));

    // step 2: Initialize & configure streams
    inflateInit_(&infStrm, ver, (int)sizeof(z_stream));
    inflateCopy(&copyStrm, &infStrm);
    unsigned char * window = (unsigned char *)malloc(1);
    memset(window, 0, 1);
    inflateBackInit_(&backStrm, 15, window, ver, (int)sizeof(z_stream));

    // step 3: Operate using inflateBack with callbacks and write a byte via gzputc
    struct InDesc { const unsigned char *data; uLong len; uLong pos; };
    struct OutDesc { unsigned char *data; uLong len; uLong pos; };
    InDesc in_desc = { compBuf, (uLong)compBufLen, 0 };
    OutDesc out_desc = { (unsigned char *)malloc((size_t)inputLen * 2), (uLong)(inputLen * 2), 0 };
    memset(out_desc.data, 0, (size_t)out_desc.len);
    auto in_cb = +[](void *d, unsigned char **buf) -> unsigned int {
        InDesc *desc = (InDesc *)d;
        uLong rem = desc->len - desc->pos;
        unsigned int provide = (unsigned int)rem;
        *buf = (unsigned char *)(desc->data + desc->pos);
        desc->pos += provide;
        return provide;
    };
    auto out_cb = +[](void *d, unsigned char *buf, unsigned int len) -> int {
        OutDesc *desc = (OutDesc *)d;
        memcpy(desc->data + desc->pos, buf, len);
        desc->pos += len;
        return 0;
    };
    inflateBack(&backStrm, (in_func)in_cb, &in_desc, (out_func)out_cb, &out_desc);
    gzFile gz = gzopen("tmp_test_zlib.gz", "wb");
    gzputc(gz, 'Z');
    gzclose(gz);

    // step 4: Validate state and cleanup
    inflateBackEnd(&backStrm);
    inflateEnd(&infStrm);
    inflateEnd(&copyStrm);
    free(input);
    free(compBuf);
    free(window);
    free(out_desc.data);

    // API sequence test completed successfully
    return 66;
}