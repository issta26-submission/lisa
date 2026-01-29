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
//<ID> 722
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / Configure
    const char *filename = "test_zlib_api_sequence_tmp.gz";
    unsigned char write_buf[256];
    memset(write_buf, 'Z', sizeof(write_buf));
    unsigned int write_len = (unsigned int)sizeof(write_buf);

    // step 2: Operate - write compressed file
    gzFile gw = gzopen(filename, "wb");
    int write_ret = gzwrite(gw, (voidp)write_buf, write_len);
    int gw_close_ret = gzclose(gw);

    // step 3: Operate - open, seek, and read using gzseek64 + gzfread
    gzFile gr = gzopen(filename, "rb");
    off64_t seek_offset = (off64_t)64;
    off64_t seek_ret = gzseek64(gr, seek_offset, 0); /* SEEK_SET == 0 */
    z_size_t read_size = (z_size_t)128;
    voidp read_buf = malloc((size_t)read_size);
    memset(read_buf, 0, (size_t)read_size);
    z_size_t items_read = gzfread(read_buf, 1, read_size, gr);

    // step 4: Validate / Cleanup
    uLong crc = crc32_z(0UL, (const Bytef *)read_buf, items_read);
    int gr_close_ret = gzclose(gr);
    free(read_buf);

    (void)filename;
    (void)write_len;
    (void)write_ret;
    (void)gw_close_ret;
    (void)seek_ret;
    (void)seek_offset;
    (void)read_size;
    (void)items_read;
    (void)crc;
    (void)gr_close_ret;
    // API sequence test completed successfully
    return 66;
}