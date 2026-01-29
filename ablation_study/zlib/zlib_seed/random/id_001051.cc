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
//<ID> 1051
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial small computations
    const char payload[] = "zlib API sequence payload exercising zlibCompileFlags, deflate, adler32_z, gzdopen";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong compile_flags = zlibCompileFlags();
    uLong adler = adler32_z(0UL, src_buf, (z_size_t)payload_len);
    uLong out_bound = compressBound((uLong)payload_len);
    Bytef *out_buf = new Bytef[(out_bound ? out_bound : 1)];
    uInt out_buf_size = (uInt)(out_bound ? out_bound : 1);

    // step 2: Initialize and configure deflate stream
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)src_buf;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = out_buf;
    dstrm.avail_out = out_buf_size;

    // step 3: Operate (deflate) and emit compressed data via gz wrapper
    int rc_deflate = deflate(&dstrm, 0);
    uLong produced = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);
    gzFile gz = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gz, out_buf, (unsigned int)produced);
    int rc_gzflush = gzflush(gz, 0);
    int rc_gzclose = gzclose(gz);

    // step 4: Cleanup and consume results
    delete [] out_buf;
    (void)compile_flags;
    (void)adler;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)produced;
    (void)rc_deflate_end;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}