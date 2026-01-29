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
//<ID> 1058
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialize source, buffers, and small metadata
    const char payload[] = "zlib API sequence payload exercising zlibCompileFlags, deflateInit_, deflate, adler32_z, gzdopen, deflateEnd, gzwrite";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *out_buf = new Bytef[(bound ? bound : 1)];
    uLong compile_flags = zlibCompileFlags();
    uLong adler_before = adler32_z(1UL, src_buf, (z_size_t)payload_len);

    // step 2: Setup deflate stream
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    dstrm.next_in = (Bytef *)src_buf;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = out_buf;
    dstrm.avail_out = (uInt)bound;
    dstrm.adler = adler_before;
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate (deflate) and write original payload to a gz stream opened from fd 1
    int rc_deflate = deflate(&dstrm, 0);
    int rc_deflate_end = deflateEnd(&dstrm);
    gzFile gz = gzdopen(1, "wb");
    int rc_gzwrite = gz ? gzwrite(gz, src_buf, (unsigned int)payload_len) : -1;
    int rc_gzclose = gz ? gzclose(gz) : -1;

    // step 4: Cleanup and finalization
    delete [] out_buf;
    (void)compile_flags;
    (void)adler_before;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}