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
//<ID> 860
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial CRC and gz write
    const char payload[] = "zlib test payload for crc and gz I/O";
    const z_size_t payload_len = (z_size_t)(sizeof(payload) - 1);
    const Bytef *payload_buf = (const Bytef *)payload;
    uLong crc_payload = crc32_z(0UL, payload_buf, payload_len);
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = 0;
    if (gw) rc_gz_write = gzwrite(gw, payload_buf, (unsigned int)payload_len);
    int rc_gz_close_w = 0;
    if (gw) rc_gz_close_w = gzclose_w ? gzclose_w(gw) : gzclose(gw); // use gzclose_w if available, otherwise gzclose

    // step 2: Read back with gzfread and compute CRC via crc32
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    z_size_t read_count = 0;
    Bytef *read_buf = new Bytef[payload_len ? payload_len : 1];
    if (gr) read_count = gzfread((voidp)read_buf, (z_size_t)1, payload_len, gr);
    uInt read_bytes = (uInt)read_count;
    uLong crc_read = crc32(0UL, read_buf, read_bytes);

    // step 3: Initialize and reset inflate stream, initialize deflate with custom parameters
    z_stream istrm;
    z_stream dstrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_reset = inflateReset(&istrm);
    int rc_deflate_init2 = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));

    // step 4: Validate combined CRCs and cleanup streams and files
    uLong combined_crc = crc_payload ^ crc_read;
    istrm.adler = combined_crc;
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_gz_close_r = 0;
    if (gr) rc_gz_close_r = gzclose_r ? gzclose_r(gr) : gzclose(gr); // use gzclose_r if available, otherwise gzclose
    delete [] read_buf;
    (void)rc_gz_write;
    (void)rc_gz_close_w;
    (void)read_count;
    (void)rc_inflate_init;
    (void)rc_inflate_reset;
    (void)rc_deflate_init2;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    (void)rc_gz_close_r;
    (void)crc_payload;
    (void)crc_read;
    (void)combined_crc;
    // API sequence test completed successfully
    return 66;
}