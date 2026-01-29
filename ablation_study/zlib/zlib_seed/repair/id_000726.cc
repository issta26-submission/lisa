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
//<ID> 726
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    char write_buf[64];
    memset(write_buf, 'Z', sizeof(write_buf));
    int write_len = (int)sizeof(write_buf);

    // step 2: Create and write compressed file
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_write_ret = gzwrite(gw, (voidp)write_buf, (unsigned int)write_len);
    int gw_close_ret = gzclose(gw);

    // step 3: Open for reading, seek with gzseek64, and read with gzfread
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off64_t seek_pos = (off64_t)5;
    off64_t seek_ret = gzseek64(gr, seek_pos, 0);
    char read_buf[32];
    memset(read_buf, 0, sizeof(read_buf));
    z_size_t read_cnt = gzfread(read_buf, (z_size_t)1, (z_size_t)16, gr);
    int gr_close_ret = gzclose(gr);

    // step 4: Validate / Cleanup
    uLong checksum_write = adler32(0UL, (const Bytef *)write_buf, (uInt)write_len);
    uLong checksum_read = adler32(0UL, (const Bytef *)read_buf, (uInt)read_cnt);
    off64_t part_len = (off64_t)read_cnt;
    uLong combined = adler32_combine64(checksum_write, checksum_read, part_len);

    (void)version;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)seek_ret;
    (void)read_cnt;
    (void)gr_close_ret;
    (void)checksum_write;
    (void)checksum_read;
    (void)combined;
    // API sequence test completed successfully
    return 66;
}