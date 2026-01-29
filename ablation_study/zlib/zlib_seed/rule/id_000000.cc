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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initial buffers
    z_stream defstrm;
    z_stream instrm;
    z_stream copystrm;
    z_stream backstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&instrm, 0, sizeof(instrm));
    memset(&copystrm, 0, sizeof(copystrm));
    memset(&backstrm, 0, sizeof(backstrm));
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    unsigned char window[32768];
    unsigned char input_buf[32] = { 0x78, 0x9c, 0x63, 0x60, 0x60, 0x64, 0x60, 0x00, 0x00, 0x06, 0x00, 0x02 }; // small zlib-like header bytes
    unsigned char output_buf[256];
    struct IoDesc { unsigned char *in; unsigned int in_len; unsigned char *out; unsigned int out_capacity; unsigned int out_used; };
    IoDesc io;
    io.in = input_buf;
    io.in_len = sizeof(input_buf);
    io.out = output_buf;
    io.out_capacity = sizeof(output_buf);
    io.out_used = 0;

    // step 2: Initialize and configure streams
    deflateInit_(&defstrm, 6, zlibVersion(), sizeof(z_stream));
    inflateInit_(&instrm, zlibVersion(), sizeof(z_stream));
    inflateBackInit_(&backstrm, 15, window, zlibVersion(), sizeof(z_stream));
    inflateCopy(&copystrm, &instrm);
    deflateResetKeep(&defstrm);

    // step 3: Operate using inflateBack with supplied callbacks
    in_func in_cb = [](void *desc, unsigned char **buf) -> unsigned int {
        IoDesc *d = reinterpret_cast<IoDesc *>(desc);
        *buf = d->in;
        unsigned int ret = d->in_len;
        d->in_len = 0;
        return ret;
    };
    out_func out_cb = [](void *desc, unsigned char *data, unsigned int len) -> int {
        IoDesc *d = reinterpret_cast<IoDesc *>(desc);
        unsigned int to_copy = len;
        unsigned int i = 0;
        for (; i < to_copy && d->out_used < d->out_capacity; ++i) {
            d->out[d->out_used++] = data[i];
        }
        return 0;
    };
    inflateBack(&backstrm, in_cb, &io, out_cb, &io);
    inflateBackEnd(&backstrm);

    // step 4: Validate-like operations and cleanup
    gzclearerr(gz);
    inflateEnd(&instrm);
    inflateEnd(&copystrm);
    deflateEnd(&defstrm);
    gzclose(gz);
    // API sequence test completed successfully
    return 66;
}