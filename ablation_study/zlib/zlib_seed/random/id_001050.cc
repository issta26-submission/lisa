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
//<ID> 1050
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial queries
    const char payload[] = "zlib API sequence payload exercising zlibCompileFlags, deflateInit_, deflate, deflateEnd, adler32_z, gzdopen and gzwrite";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong compile_flags = zlibCompileFlags();
    uLong checksum = adler32_z(0UL, src_buf, (z_size_t)payload_len);

    // step 2: Prepare output buffer (based on compressBound) and initialize deflate stream
    uLong bound = compressBound(payload_len);
    Bytef *out_buf = new Bytef[(bound ? bound : 1)];
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)src_buf;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = out_buf;
    dstrm.avail_out = (uInt)bound;

    // step 3: Operate (deflate) and finalize stream
    int rc_deflate = deflate(&dstrm, 0);
    uLong produced = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 4: Write compressed output to a gzFile obtained from an existing fd, flush and cleanup
    gzFile gzf = gzdopen(1, "wb");
    int rc_gzwrite = 0;
    int rc_gzflush = 0;
    int rc_gzclose = 0;
    if (gzf) {
        rc_gzwrite = gzwrite(gzf, out_buf, (unsigned int)(produced ? produced : 0));
        rc_gzflush = gzflush(gzf, 0);
        rc_gzclose = gzclose(gzf);
    }
    delete [] out_buf;

    (void)compile_flags;
    (void)checksum;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}