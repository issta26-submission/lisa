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
//<ID> 1055
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload exercising zlibCompileFlags, deflateInit_, deflate, adler32_z, gzdopen, gzwrite, deflateEnd";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef * src = (const Bytef *)payload;
    uLong compile_flags = zlibCompileFlags();

    // step 2: Initialize and configure deflate stream, allocate buffers
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)src;
    dstrm.avail_in = (uInt)payload_len;
    uLong out_bound = deflateBound(&dstrm, (uLong)payload_len);
    Bytef * outbuf = new Bytef[(out_bound ? out_bound : 1)];
    dstrm.next_out = outbuf;
    dstrm.avail_out = (uInt)(out_bound ? out_bound : 1);

    // step 3: Operate — perform deflate, compute adler32 on source, and finalize deflate
    int rc_deflate = deflate(&dstrm, 0);
    uLong checksum = adler32_z(1UL, src, (z_size_t)payload_len);
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 4: Use gzdopen to write compressed output to a gz stream and cleanup
    gzFile gz = gzdopen(1, "wb");
    int written = gzwrite(gz, outbuf, (unsigned int)(dstrm.total_out ? (uLong)dstrm.total_out : payload_len));
    int rc_gzflush = gzflush(gz, 0);
    int rc_gzclose = gzclose(gz);
    delete [] outbuf;

    (void)compile_flags;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)checksum;
    (void)rc_deflate_end;
    (void)written;
    (void)rc_gzflush;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}