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
//<ID> 721
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    unsigned int write_len = 512U;
    unsigned char *src = (unsigned char *)malloc((size_t)write_len);
    memset(src, 'A', (size_t)write_len);
    unsigned char *read_buf = (unsigned char *)malloc((size_t)write_len);
    memset(read_buf, 0, (size_t)write_len);

    // step 2: Configure (create and write a gz file)
    gzFile gzw = gzopen(fname, "wb");
    int gz_write_ret = gzwrite(gzw, src, write_len);
    int gz_close_write_ret = gzclose(gzw);

    // step 3: Operate (open, seek using gzseek64, and read using gzfread)
    gzFile gzr = gzopen(fname, "rb");
    off64_t seek_offset = (off64_t)128;
    off64_t new_pos = gzseek64(gzr, seek_offset, 0); // 0 == SEEK_SET
    z_size_t to_read = (z_size_t)(write_len - (unsigned int)seek_offset);
    z_size_t items_read = gzfread((voidp)read_buf, (z_size_t)1, to_read, gzr);
    int gz_close_read_ret = gzclose(gzr);

    // step 4: Validate / Cleanup
    uLong crc_full = crc32(0UL, (const Bytef *)src, (uInt)write_len);
    uLong crc_partial = crc32(0UL, (const Bytef *)read_buf, (uInt)items_read);
    free(src);
    free(read_buf);
    (void)fname;
    (void)gz_write_ret;
    (void)gz_close_write_ret;
    (void)new_pos;
    (void)seek_offset;
    (void)items_read;
    (void)gz_close_read_ret;
    (void)crc_full;
    (void)crc_partial;
    // API sequence test completed successfully
    return 66;
}