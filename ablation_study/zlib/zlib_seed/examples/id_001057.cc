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
//<ID> 1057
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and Initialize
    const char payload[] = "zlib API sequence payload exercising zlibCompileFlags, adler32_z, deflate, deflateEnd, gzdopen and gzwrite";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong compile_flags = zlibCompileFlags();
    uLong initial_adler = adler32_z(0UL, src_buf, (z_size_t)payload_len);

    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong bound = compressBound(payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    dstrm.next_in = (Bytef *)src_buf;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(bound ? bound : 1);

    // step 2: Operate (deflate stream)
    int rc_deflate = deflate(&dstrm, 0);
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Output compressed data to a gzFile and compute checksum over compressed output
    gzFile gz = gzdopen(1, "wb");
    unsigned int out_len = (unsigned int)(dstrm.total_out ? dstrm.total_out : (uLong)0);
    int rc_gzwrite = gzwrite(gz, comp_buf, out_len);
    int rc_gzflush = gzflush(gz, 0);
    int rc_gzclose = gzclose(gz);
    uLong comp_adler = adler32_z(0UL, comp_buf, (z_size_t)dstrm.total_out);

    // step 4: Cleanup and finalize
    delete [] comp_buf;
    (void)compile_flags;
    (void)initial_adler;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)gz;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose;
    (void)comp_adler;
    // API sequence test completed successfully
    return 66;
}