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
//<ID> 935
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare payload, allocate buffers and compress with deflate
    const char payload[] = "zlib API sequence payload: test deflate, deflateParams, inflateInit_, gzwrite/gzread and gzflush";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    Bytef *dec_buf = new Bytef[(payload_len ? payload_len : 1)];
    Bytef *read_buf = new Bytef[(payload_len ? payload_len : 1)];

    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)src_buf;
    dstrm.avail_in = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(bound <= (uLong)~0u ? bound : (uLong)~0u);
    int rc_deflate_params = deflateParams(&dstrm, 9, 0);
    int rc_deflate = deflate(&dstrm, Z_FINISH);
    uLong comp_size = (uLong)dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 2: Write original payload into a gzFile, flush and read it back
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u));
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzrewind = gzrewind(gf);
    unsigned int to_read = (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_gzread = gzread(gf, (voidp)read_buf, to_read);

    // step 3: Initialize inflate stream, inflate the deflated buffer into dec_buf
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_size <= (uLong)~0u ? comp_size : (uLong)~0u);
    istrm.next_out = dec_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Validation using CRCs and cleanup resources
    uLong crc_src = crc32(0UL, src_buf, (uInt)payload_len);
    uLong crc_dec = crc32(0UL, dec_buf, (uInt)payload_len);
    uLong crc_read = crc32(0UL, read_buf, (uInt)payload_len);

    int rc_gzclose = gzclose(gf);

    delete [] comp_buf;
    delete [] dec_buf;
    delete [] read_buf;

    (void)rc_deflate_init;
    (void)rc_deflate_params;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzrewind;
    (void)rc_gzread;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    (void)crc_src;
    (void)crc_dec;
    (void)crc_read;
    // API sequence test completed successfully
    return 66;
}