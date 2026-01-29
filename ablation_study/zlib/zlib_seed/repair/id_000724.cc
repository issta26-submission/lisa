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
//<ID> 724
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    unsigned char write_buf[128];
    memset(write_buf, 'Z', sizeof(write_buf));
    unsigned int write_len = (unsigned int)sizeof(write_buf);

    // step 2: Configure (create and write a gz file)
    gzFile gw = gzopen(fname, "wb");
    int gzwrite_ret = gw ? gzwrite(gw, (voidp)write_buf, write_len) : -1;
    int gzclose_write_ret = gw ? gzclose(gw) : -1;

    // step 3: Operate (open for read, seek with gzseek64, and read with gzfread)
    gzFile gr = gzopen(fname, "rb");
    off64_t seek_offset = (off64_t)64;
    off64_t newpos = gr ? gzseek64(gr, seek_offset, 0) : (off64_t)-1;
    unsigned char read_buf[64];
    memset(read_buf, 0, sizeof(read_buf));
    z_size_t nitems_read = gr ? gzfread(read_buf, (z_size_t)1, (z_size_t)sizeof(read_buf), gr) : (z_size_t)0;
    int gzclose_read_ret = gr ? gzclose(gr) : -1;

    // step 4: Validate / Cleanup
    (void)fname;
    (void)write_len;
    (void)gzwrite_ret;
    (void)gzclose_write_ret;
    (void)newpos;
    (void)nitems_read;
    (void)gzclose_read_ret;
    (void)read_buf;
    (void)write_buf;
    // API sequence test completed successfully
    return 66;
}