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
//<ID> 865
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial file write
    const char filename[] = "test_zlib_api_sequence_tmp.gz";
    const char payload[] = "zlib api sequence payload for gzfread and crc";
    const z_size_t payload_len = (z_size_t)(sizeof(payload) - 1);
    const Bytef * payload_buf = (const Bytef *)payload;
    gzFile gw = gzopen(filename, "wb");
    int rc_gz_write = 0;
    if (gw) rc_gz_write = gzwrite(gw, payload_buf, (unsigned int)payload_len);
    int rc_gz_close_w = gzclose(gw);

    // step 2: Initialize and configure zlib streams
    z_stream defstrm;
    z_stream instrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    memset(&instrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init2 = deflateInit2_(&defstrm, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&instrm, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_reset = inflateReset(&instrm);

    // step 3: Operate - read from gz file, compute CRCs
    gzFile gr = gzopen(filename, "rb");
    Bytef * read_buf = new Bytef[payload_len ? payload_len : 1];
    z_size_t items_read = gzfread((voidp)read_buf, (z_size_t)1, payload_len, gr);
    uLong crc_z_val = crc32_z(0UL, read_buf, items_read);
    uLong crc_val = crc32(0UL, read_buf, (uInt)items_read);

    // step 4: Validate and Cleanup
    instrm.adler = crc_z_val;
    defstrm.adler = crc_val;
    int rc_deflate_end = deflateEnd(&defstrm);
    int rc_inflate_end = inflateEnd(&instrm);
    int rc_gz_close_r = gzclose(gr);
    delete [] read_buf;
    (void)rc_gz_write;
    (void)rc_gz_close_w;
    (void)rc_deflate_init2;
    (void)rc_inflate_init;
    (void)rc_inflate_reset;
    (void)items_read;
    (void)crc_z_val;
    (void)crc_val;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    (void)rc_gz_close_r;
    // API sequence test completed successfully
    return 66;
}