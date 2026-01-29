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
//<ID> 1056
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and basic computations
    const char payload[] = "zlib API sequence payload exercising zlibCompileFlags, deflate, adler32_z and gzdopen";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong compile_flags = zlibCompileFlags();
    uLong payload_adler = adler32_z(0UL, src_buf, (z_size_t)payload_len);

    // step 2: Initialize deflate stream and allocate output buffer
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    dstrm.next_in = (Bytef *)src_buf;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.adler = payload_adler;
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong out_bound = compressBound((uLong)payload_len);
    Bytef *out_buf = new Bytef[(out_bound ? out_bound : 1)];
    dstrm.next_out = out_buf;
    dstrm.avail_out = (uInt)(out_bound ? out_bound : 1);

    // step 3: Operate — perform deflate, query pending, and finalize; also write original data to gz stream
    int rc_deflate = deflate(&dstrm, 0);
    unsigned int pending_bytes = 0;
    int pending_bits = 0;
    int rc_pending = deflatePending(&dstrm, &pending_bytes, &pending_bits);
    int rc_deflate_end = deflateEnd(&dstrm);
    gzFile gzf = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gzf, src_buf, (unsigned int)payload_len);
    int rc_gzflush = gzflush(gzf, 0);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup and finalization
    delete [] out_buf;
    (void)compile_flags;
    (void)payload_adler;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)pending_bytes;
    (void)pending_bits;
    (void)rc_pending;
    (void)rc_deflate_end;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}