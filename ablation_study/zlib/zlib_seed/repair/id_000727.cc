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
//<ID> 727
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *filename = "test_zlib_api_sequence_tmp.gz";
    unsigned char write_buf[128];
    memset(write_buf, 'A', sizeof(write_buf));
    gzFile gw = gzopen(filename, "wb");
    int written = gzwrite(gw, write_buf, (unsigned int)sizeof(write_buf));
    int close_written = gzclose(gw);

    // step 2: Configure
    gzFile gr = gzopen(filename, "rb");

    // step 3: Operate
    off64_t new_pos = gzseek64(gr, (off64_t)10, 0);
    unsigned char read_buf[64];
    memset(read_buf, 0, sizeof(read_buf));
    z_size_t items_read = gzfread(read_buf, (z_size_t)1, (z_size_t)sizeof(read_buf), gr);

    // step 4: Validate / Cleanup
    uLong checksum = adler32(0UL, read_buf, (uInt)items_read);
    int close_read = gzclose(gr);

    (void)written;
    (void)close_written;
    (void)new_pos;
    (void)items_read;
    (void)checksum;
    (void)close_read;
    // API sequence test completed successfully
    return 66;
}