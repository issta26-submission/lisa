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
//<ID> 572
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef plain_in[128];
    memset(plain_in, 'A', sizeof(plain_in));
    Bytef comp_buf[512];
    memset(comp_buf, 0, sizeof(comp_buf));
    uLongf comp_buf_len = (uLongf)sizeof(comp_buf);
    int comp_ret = compress(comp_buf, &comp_buf_len, plain_in, (uLong)sizeof(plain_in));
    uLong comp_len = (uLong)comp_buf_len;

    // step 2: Configure / Write compressed data using gzopen and gzseek
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_write = gzwrite(gw, comp_buf, (unsigned int)comp_len);
    off_t gw_seek_pos = (off_t)0;
    off_t gw_seek_ret = gzseek(gw, gw_seek_pos, 0); /* 0 == SEEK_SET */
    int gw_close_ret = gzclose(gw);

    // step 3: Open with gzopen64 and read back, using gzseek
    gzFile gr = gzopen64("test_zlib_api_sequence_tmp.gz", "rb");
    off_t gr_seek_ret = gzseek(gr, (off_t)0, 0); /* seek to start */
    Bytef read_buf[512];
    memset(read_buf, 0, sizeof(read_buf));
    int gr_read = gzread(gr, read_buf, (unsigned int)sizeof(read_buf));
    int gr_close_ret = gzclose(gr);

    // step 4: Validate / Cleanup
    uLong crc_orig = crc32(0UL, plain_in, (uInt)sizeof(plain_in));
    /* compute CRC of what was written (use comp_len) */
    uLong crc_written = crc32(0UL, comp_buf, (uInt)comp_len);
    /* compute CRC of what was read (use gr_read if positive, otherwise 0) */
    uLong crc_read = crc32(0UL, read_buf, (uInt)(gr_read > 0 ? (unsigned int)gr_read : 0u));

    (void)ver;
    (void)comp_ret;
    (void)comp_len;
    (void)gw_write;
    (void)gw_seek_ret;
    (void)gw_close_ret;
    (void)gr_seek_ret;
    (void)gr_read;
    (void)gr_close_ret;
    (void)crc_orig;
    (void)crc_written;
    (void)crc_read;
    // API sequence test completed successfully
    return 66;
}