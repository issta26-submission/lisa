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
//<ID> 936
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and stream initialization
    const char payload[] = "zlib API sequence payload for combined usage of deflate/inflate and gzFile I/O";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound(payload_len);
    Bytef *comp_buf = new Bytef[(size_t)bound];
    Bytef *uncmp_buf = new Bytef[(size_t)payload_len];
    Bytef *read_buf = new Bytef[(size_t)payload_len];
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure deflate stream parameters
    int rc_deflate_params = deflateParams(&dstrm, 9, 1);

    // step 3: Operate - compress with compress2, inflate the compressed data, write/read with gzFile
    uLongf comp_len_f = (uLongf)bound;
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, payload_len, 6);
    uLong comp_size = (uLong)comp_len_f;
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)payload_len;
    int rc_inflate = inflate(&istrm, 0);

    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzwrite = gzwrite(gf, (voidpc)uncmp_buf, (unsigned int)(payload_len));
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzrewind = gzrewind(gf);
    int rc_gzread = gzread(gf, (voidp)read_buf, (unsigned int)(payload_len));

    // step 4: Validate and cleanup
    uLong crc_src = crc32(0UL, src_buf, (uInt)payload_len);
    uLong crc_read = crc32(0UL, read_buf, (uInt)payload_len);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gf);
    delete [] comp_buf;
    delete [] uncmp_buf;
    delete [] read_buf;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)rc_deflate_params;
    (void)rc_compress;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzrewind;
    (void)rc_gzread;
    (void)crc_src;
    (void)crc_read;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}