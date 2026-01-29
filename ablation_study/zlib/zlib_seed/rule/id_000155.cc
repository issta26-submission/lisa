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
//<ID> 155
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    unsigned char input[128];
    unsigned char output[256];
    memset(input, 'A', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags;

    // step 2: Setup (deflate raw data into compBuf; prepare inflate back)
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    deflateInit2_(&def_strm, 6, 8, -15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)bound;
    deflate(&def_strm, 0);

    // step 3: Core operations (initialize inflate for raw deflate and run inflateBack with callbacks)
    struct IO_desc { unsigned char *in; unsigned int in_size; unsigned int in_pos; unsigned char *out; unsigned int out_size; unsigned int out_pos; };
    IO_desc io;
    io.in = compBuf;
    io.in_size = (unsigned int)def_strm.total_out;
    io.in_pos = 0;
    io.out = output;
    io.out_size = (unsigned int)sizeof(output);
    io.out_pos = 0;

    in_func in_cb = [](void *in_desc, unsigned char **buf) -> unsigned int {
        IO_desc *d = (IO_desc *)in_desc;
        unsigned int avail = d->in_size - d->in_pos;
        *buf = d->in + d->in_pos;
        d->in_pos += avail;
        return avail;
    };

    out_func out_cb = [](void *out_desc, unsigned char *buf, unsigned int len) -> int {
        IO_desc *d = (IO_desc *)out_desc;
        unsigned int available = d->out_size - d->out_pos;
        unsigned int copy = (len <= available) ? len : available;
        memcpy(d->out + d->out_pos, buf, copy);
        d->out_pos += copy;
        return 0;
    };

    inf_strm.zalloc = Z_NULL;
    inf_strm.zfree = Z_NULL;
    inf_strm.opaque = Z_NULL;
    inflateInit2_(&inf_strm, -15, zlibVersion(), (int)sizeof(z_stream));
    inflateBack(&inf_strm, in_cb, &io, out_cb, &io);

    // step 4: Validate & Cleanup
    uLong checksum = adler32(0L, output, (uInt)io.out_pos);
    (void)checksum;
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    free(compBuf);
    // API sequence test completed successfully
    return 66;
}