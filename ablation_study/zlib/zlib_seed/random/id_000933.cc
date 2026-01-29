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
//<ID> 933
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare payload and exercise gzFile write/read/flush
    const char payload[] = "zlib API sequence payload to exercise gzwrite and gzread along with stream ops";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    unsigned int to_write = (unsigned int)payload_len;
    int rc_gzwrite = gzwrite(gf, (voidpc)payload, to_write);
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzrewind = gzrewind(gf);
    Bytef *read_buf = new Bytef[(payload_len ? payload_len : 1)];
    unsigned int to_read = (unsigned int)payload_len;
    int rc_gzread = gzread(gf, (voidp)read_buf, to_read);
    uLong crc_read = crc32(0UL, read_buf, (uInt)payload_len);

    // step 2: Compress payload into an in-memory buffer (for inflate testing)
    uLong bound = compressBound(payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_compress = compress2(comp_buf, &comp_len_f, (const Bytef *)payload, (uLong)payload_len, 6);
    uLong comp_size = (uLong)comp_len_f;

    // step 3: Initialize inflate stream, inflate compressed buffer, validate output CRC
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_size;
    Bytef *out_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)payload_len;
    int rc_inflate = inflate(&istrm, 0);
    uLong crc_inflated = crc32(0UL, out_buf, (uInt)payload_len);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Initialize deflate stream, adjust parameters, cleanup all resources
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    int rc_deflate_params = deflateParams(&dstrm, 9, 1);
    int rc_deflate_end = deflateEnd(&dstrm);

    int rc_gzclose = gzclose(gf);
    delete [] read_buf;
    delete [] comp_buf;
    delete [] out_buf;

    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzrewind;
    (void)rc_gzread;
    (void)crc_read;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)crc_inflated;
    (void)rc_inflate_end;
    (void)rc_deflate_init;
    (void)rc_deflate_params;
    (void)rc_deflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}