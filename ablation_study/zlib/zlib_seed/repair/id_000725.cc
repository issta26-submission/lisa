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
//<ID> 725
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef *write_buf = (Bytef *)malloc(256);
    memset(write_buf, 'Z', 256);
    unsigned int write_len = 256U;

    // step 2: Configure (create and write a gzipped file)
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_write_ret = 0;
    if (gw) gz_write_ret = gzwrite(gw, write_buf, (unsigned int)write_len);
    int gw_close_ret = 0;
    if (gw) gw_close_ret = gzclose(gw);

    // step 3: Operate (open, seek with gzseek64, and read a portion with gzfread)
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off64_t seek_ret = 0;
    if (gr) seek_ret = gzseek64(gr, (off64_t)128, 0);
    z_size_t read_count = 128;
    Bytef *read_buf = (Bytef *)malloc((size_t)read_count + 1);
    memset(read_buf, 0, (size_t)read_count + 1);
    z_size_t items_read = 0;
    if (gr) items_read = gzfread(read_buf, (z_size_t)1, read_count, gr);
    int gr_close_ret = 0;
    if (gr) gr_close_ret = gzclose(gr);

    // step 4: Validate / Cleanup
    uLong checksum = adler32(0UL, read_buf, (uInt)items_read);
    free(write_buf);
    free(read_buf);
    (void)version;
    (void)gz_write_ret;
    (void)gw_close_ret;
    (void)seek_ret;
    (void)items_read;
    (void)gr_close_ret;
    (void)checksum;
    // API sequence test completed successfully
    return 66;
}