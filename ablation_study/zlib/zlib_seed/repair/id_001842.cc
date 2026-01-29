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
//<ID> 1842
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef original[] = "zlib API sequence test: write gz -> rewind -> gzfread -> crc32 -> inflateInit_";
    uLong original_len = (uLong)(sizeof(original) - 1);
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    const char *ver = zlibVersion();
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));

    // step 2: Configure (write compressed data into a gz file)
    gzFile gz_out = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gzwrite_ret = gzwrite(gz_out, original, (unsigned int)original_len);
    int gzclose_out_ret = gzclose(gz_out);

    // step 3: Operate (open for read, rewind, and read back with gzfread)
    gzFile gz_in = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int gzrewind_ret = gzrewind(gz_in);
    Bytef *read_buf = (Bytef *)malloc((size_t)(original_len + 16));
    memset(read_buf, 0, (int)(original_len + 16));
    z_size_t items_read = gzfread(read_buf, (z_size_t)1, (z_size_t)original_len, gz_in);

    // step 4: Validate & Cleanup
    uLong crc_original = crc32(0UL, original, (uInt)original_len);
    uLong crc_read = crc32(0UL, read_buf, (uInt)original_len);
    int inf_end_ret = inflateEnd(&inf_strm);
    int gzclose_in_ret = gzclose(gz_in);
    free(read_buf);
    (void)ver;
    (void)inf_init_ret;
    (void)gzwrite_ret;
    (void)gzclose_out_ret;
    (void)gzrewind_ret;
    (void)items_read;
    (void)crc_original;
    (void)crc_read;
    (void)inf_end_ret;
    (void)gzclose_in_ret;
    // API sequence test completed successfully
    return 66;
}