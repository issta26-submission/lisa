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
//<ID> 414
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize input and deflate stream
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    deflateInit_(&deflate_strm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure deflate stream and perform deflate
    deflate_strm.next_in = (Bytef *)input;
    deflate_strm.avail_in = (uInt)inputLen;
    deflate_strm.next_out = compBuf;
    deflate_strm.avail_out = (uInt)bound;
    deflate(&deflate_strm, 0);

    // step 3: Initialize inflate-back stream, copy it, and get header
    z_stream back_strm;
    memset(&back_strm, 0, sizeof(back_strm));
    unsigned char window[1 << 15];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&back_strm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    z_stream copy_strm;
    memset(&copy_strm, 0, sizeof(copy_strm));
    inflateCopy(&copy_strm, &back_strm);
    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&copy_strm, &header);

    // step 4: Use inflateBack with callbacks to decompress, then cleanup
    unsigned char * decompBuf = (unsigned char *)malloc((size_t)inputLen);
    memset(decompBuf, 0, (size_t)inputLen);
    struct InDesc { const unsigned char * data; unsigned int size; unsigned int pos; };
    struct OutDesc { unsigned char * data; unsigned int size; unsigned int pos; };
    InDesc in_desc;
    in_desc.data = compBuf;
    in_desc.size = (unsigned int)deflate_strm.total_out;
    in_desc.pos = 0;
    OutDesc out_desc;
    out_desc.data = decompBuf;
    out_desc.size = (unsigned int)inputLen;
    out_desc.pos = 0;
    in_func in_cb = [](void * desc, unsigned char ** buf) -> unsigned int {
        InDesc * d = (InDesc *)desc;
        unsigned int remaining = d->size - d->pos;
        *buf = (unsigned char *)(d->data + d->pos);
        d->pos += remaining;
        return remaining;
    };
    out_func out_cb = [](void * desc, unsigned char * buf, unsigned int len) -> int {
        OutDesc * o = (OutDesc *)desc;
        memcpy(o->data + o->pos, buf, (size_t)len);
        o->pos += len;
        return 0;
    };
    inflateBack(&back_strm, in_cb, (void *)&in_desc, out_cb, (void *)&out_desc);
    inflateBackEnd(&back_strm);
    inflateEnd(&copy_strm);
    deflateEnd(&deflate_strm);
    free(compBuf);
    free(decompBuf);
    return 66;
    // API sequence test completed successfully
}