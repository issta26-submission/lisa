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
//<ID> 938
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and buffer allocation
    const char payload[] = "zlib API sequence payload for deflateParams, inflateInit_, gzwrite/gzread and gzflush";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef * src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef * comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    Bytef * uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    Bytef * read_buf = new Bytef[(payload_len ? payload_len : 1)];

    // step 2: Compress payload (using compress2) and prepare deflate stream for parameter tuning
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);
    uLong comp_size = (uLong)comp_len_f;
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    int rc_deflate_params = deflateParams(&dstrm, 9, 0);
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Initialize inflate stream, perform inflate of compressed buffer
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_size <= (uLong)~0u ? comp_size : (uLong)~0u);
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Use gzFile to write flushed data and read it back, then cleanup
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzwrite = gzwrite(gf, (voidpc)uncmp_buf, (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u));
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzrewind = gzrewind(gf);
    int rc_gzread = gzread(gf, (voidp)read_buf, (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u));
    int rc_gzclose = gzclose(gf);

    delete [] comp_buf;
    delete [] uncmp_buf;
    delete [] read_buf;

    (void)rc_compress;
    (void)comp_size;
    (void)rc_deflate_init;
    (void)rc_deflate_params;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzrewind;
    (void)rc_gzread;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}