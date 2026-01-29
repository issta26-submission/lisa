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
//<ID> 861
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and write compressed file
    const char payload[] = "sample data for zlib sequence";
    const z_size_t payload_len = (z_size_t)(sizeof(payload) - 1);
    const Bytef *pbuf = (const Bytef *)payload;
    uLong crc_a = crc32(0UL, pbuf, (uInt)payload_len);
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    z_size_t written = gzfwrite((voidpc)pbuf, 1, payload_len, gw);
    int rc_gw_close = gzclose(gw);

    // step 2: Read gz and compute CRCs
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *readBuf = new Bytef[payload_len ? payload_len : 1];
    z_size_t readCount = gzfread((voidp)readBuf, 1, payload_len, gr);
    uLong crc_b = crc32_z(0UL, readBuf, readCount);
    uLong crc_c = crc32(0UL, readBuf, (uInt)readCount);

    // step 3: Initialize and configure streams
    z_stream defl;
    memset(&defl, 0, (size_t)sizeof(z_stream));
    int rc_defl_init = deflateInit2_(&defl, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    defl.next_in = readBuf;
    defl.avail_in = (uInt)readCount;
    defl.next_out = readBuf;
    defl.avail_out = (uInt)payload_len;
    z_stream infl;
    memset(&infl, 0, (size_t)sizeof(z_stream));
    int rc_infl_init = inflateInit_(&infl, zlibVersion(), (int)sizeof(z_stream));
    int rc_infl_reset = inflateReset(&infl);

    // step 4: Validate and Cleanup
    uLong combined_crc = crc_a ^ crc_b ^ crc_c;
    defl.adler = combined_crc;
    infl.adler = combined_crc;
    int rc_defl_end = deflateEnd(&defl);
    int rc_infl_end = inflateEnd(&infl);
    int rc_gr_close = gzclose(gr);
    delete [] readBuf;
    (void)written;
    (void)rc_gw_close;
    (void)crc_a;
    (void)crc_b;
    (void)crc_c;
    (void)rc_defl_init;
    (void)rc_infl_init;
    (void)rc_infl_reset;
    (void)combined_crc;
    (void)rc_defl_end;
    (void)rc_infl_end;
    (void)rc_gr_close;
    // API sequence test completed successfully
    return 66;
}