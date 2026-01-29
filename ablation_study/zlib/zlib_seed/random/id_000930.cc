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
//<ID> 930
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare payload and compress into a buffer
    const char payload[] = "zlib API sequence payload to exercise compress, gzwrite/gzread, inflateInit_, gzflush and deflateParams";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound(payload_len);
    uLongf comp_len_f = (uLongf)bound;
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);
    uLong comp_size = (uLong)comp_len_f;

    // step 2: Write compressed data to a gzFile, flush, rewind and read it back
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    unsigned int write_len = (unsigned int)(comp_size <= (uLong)~0u ? comp_size : (uLong)~0u);
    int rc_gzwrite = gzwrite(gf, (voidpc)comp_buf, write_len);
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzrewind = gzrewind(gf);
    Bytef *read_comp = new Bytef[(comp_size ? comp_size : 1)];
    unsigned int read_len = write_len;
    int rc_gzread = gzread(gf, (voidp)read_comp, read_len);

    // step 3: Initialize inflate stream and decompress the read compressed data
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = read_comp;
    istrm.avail_in = (uInt)(comp_size <= (uLong)~0u ? comp_size : (uLong)~0u);
    Bytef *out_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Initialize a deflate stream, adjust its parameters, then clean up all resources
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    int rc_deflate_params = deflateParams(&dstrm, 9, 1);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gf);

    delete [] comp_buf;
    delete [] read_comp;
    delete [] out_buf;

    (void)rc_compress;
    (void)comp_size;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzrewind;
    (void)rc_gzread;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_deflate_init;
    (void)rc_deflate_params;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}