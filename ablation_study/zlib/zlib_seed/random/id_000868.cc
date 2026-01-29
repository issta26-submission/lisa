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
//<ID> 868
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib sequence test: write gz, read back, CRC and stream reset";
    const z_size_t payload_len = (z_size_t)(sizeof(payload) - 1);
    const Bytef *payload_buf = (const Bytef *)payload;
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = 0;
    if (gw) {
        rc_gzwrite = gzwrite(gw, payload_buf, (unsigned int)payload_len);
        gzclose(gw);
    }

    // step 2: Configure deflate stream
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init2 = deflateInit2_(&strm, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate - read gz file, compute CRCs, reset stream
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *readBuf = new Bytef[payload_len ? payload_len : 1];
    z_size_t items_read = 0;
    if (gr) {
        items_read = gzfread((voidp)readBuf, (z_size_t)1, payload_len, gr);
    }
    uLong crc_orig = crc32_z(0UL, payload_buf, payload_len);
    uLong crc_read = crc32(0UL, readBuf, (uInt)items_read);
    int rc_inflate_reset = inflateReset(&strm);
    strm.adler = crc_orig ^ crc_read;

    // step 4: Cleanup and finalize
    int rc_deflate_end = deflateEnd(&strm);
    int rc_gzclose_r = 0;
    if (gr) {
        rc_gzclose_r = gzclose(gr);
    }
    delete [] readBuf;
    (void)rc_gzwrite;
    (void)rc_deflate_init2;
    (void)items_read;
    (void)crc_orig;
    (void)crc_read;
    (void)rc_inflate_reset;
    (void)rc_deflate_end;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}