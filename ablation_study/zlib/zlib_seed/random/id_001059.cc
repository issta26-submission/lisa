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
//<ID> 1059
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialize payload, compute flags and adler32
    const char payload[] = "zlib API sequence payload exercising zlibCompileFlags, adler32_z, compress2, gzdopen, deflateInit_, deflate, deflateEnd";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong compile_flags = zlibCompileFlags();
    uLong adler = adler32_z(0UL, src_buf, (z_size_t)payload_len);

    // step 2: Compress payload with compress2
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_comp = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);

    // step 3: Write compressed data to a gzFile obtained via gzdopen, then setup deflate stream and operate
    gzFile gf = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gf, comp_buf, (unsigned int)comp_len_f);
    int rc_gzclose = gzclose(gf);

    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = comp_buf;
    dstrm.avail_in = (uInt)comp_len_f;
    Bytef *out_buf = new Bytef[(payload_len ? payload_len : 1)];
    dstrm.next_out = out_buf;
    dstrm.avail_out = (uInt)payload_len;
    int rc_deflate = deflate(&dstrm, 0);
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 4: Cleanup and finalize
    delete [] comp_buf;
    delete [] out_buf;
    (void)compile_flags;
    (void)adler;
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}