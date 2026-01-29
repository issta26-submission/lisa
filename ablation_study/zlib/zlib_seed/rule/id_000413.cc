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
//<ID> 413
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize deflate stream
    z_stream def_strm;
    z_stream inflate_strm;
    z_stream inflate_copy_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    memset(&inflate_copy_strm, 0, sizeof(inflate_copy_strm));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure deflate stream and perform deflate
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)bound;
    deflate(&def_strm, 0);

    // step 3: Initialize inflate (back) stream, copy it, and get header
    unsigned char * windowBuf = (unsigned char *)malloc(1 << 15);
    memset(windowBuf, 0, (1 << 15));
    inflateBackInit_(&inflate_strm, 15, windowBuf, zlibVersion(), (int)sizeof(z_stream));
    inflateCopy(&inflate_copy_strm, &inflate_strm);
    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&inflate_copy_strm, &header);

    // step 4: Use inflateBack with simple in/out callbacks, then cleanup
    struct InDesc { const unsigned char * data; unsigned int len; unsigned int pos; };
    struct OutDesc { unsigned char * out; unsigned int out_len; unsigned int pos; };
    InDesc in_desc;
    in_desc.data = compBuf;
    in_desc.len = (unsigned int)def_strm.total_out;
    in_desc.pos = 0;
    unsigned char * decompressed = (unsigned char *)malloc((size_t)inputLen);
    memset(decompressed, 0, (size_t)inputLen);
    OutDesc out_desc;
    out_desc.out = decompressed;
    out_desc.out_len = (unsigned int)inputLen;
    out_desc.pos = 0;
    auto in_lambda = [](void * d, unsigned char ** buf) -> unsigned int {
        InDesc * id = (InDesc *)d;
        *buf = (unsigned char *)(id->data + id->pos);
        unsigned int avail = id->len - id->pos;
        id->pos = id->len;
        return avail;
    };
    auto out_lambda = [](void * d, unsigned char * buf, unsigned int len) -> int {
        OutDesc * od = (OutDesc *)d;
        unsigned int space = od->out_len - od->pos;
        unsigned int to_copy = (space < len) ? space : len;
        memcpy(od->out + od->pos, buf, to_copy);
        od->pos += to_copy;
        return 0;
    };
    in_func in_cb = static_cast<in_func>(in_lambda);
    out_func out_cb = static_cast<out_func>(out_lambda);
    inflateBack(&inflate_strm, in_cb, (void *)&in_desc, out_cb, (void *)&out_desc);
    inflateBackEnd(&inflate_strm);
    inflateEnd(&inflate_copy_strm);
    deflateEnd(&def_strm);
    free(compBuf);
    free(windowBuf);
    free(decompressed);
    // API sequence test completed successfully
    return 66;
}