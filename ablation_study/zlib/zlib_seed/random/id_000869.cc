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
//<ID> 869
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib sequence payload for crc and gz I/O";
    const z_size_t payload_len = (z_size_t)(sizeof(payload) - 1);
    const Bytef *payload_buf = (const Bytef *)payload;
    uLong crc_initial = crc32_z(0UL, payload_buf, payload_len);

    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Configure deflate stream and write payload to a gzip file
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)payload_buf;
    dstrm.avail_in = (uInt)payload_len;
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gw, (voidpc)payload_buf, (unsigned int)payload_len);
    int rc_gz_close_w = gzclose_w ? gzclose_w(gw) : gzclose(gw);

    // step 3: Operate - read back with gzfread and reset inflate stream
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *read_buf = new Bytef[payload_len ? payload_len : 1];
    z_size_t items_read = gzfread((voidp)read_buf, 1, payload_len, gr);
    uLong crc_read = crc32(0UL, read_buf, (uInt)items_read);

    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)items_read;
    int rc_inflate_reset = inflateReset(&istrm);

    // step 4: Validate and cleanup
    (void)rc_deflate_init;
    (void)rc_gz_write;
    (void)rc_gz_close_w;
    (void)rc_inflate_init;
    (void)rc_inflate_reset;
    (void)crc_initial;
    (void)crc_read;

    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gz_close_r = gzclose(gr);
    delete [] read_buf;

    // API sequence test completed successfully
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    (void)rc_gz_close_r;
    return 66;
}