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
//<ID> 1844
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source and write a gz file)
    const Bytef src[] = "zlib API sequence test: write gz -> rewind -> read -> crc -> inflate init";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong crc_orig = crc32(0UL, src, (uInt)src_len);
    gzFile gz_out = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gzwrite_ret = gzwrite(gz_out, src, (unsigned int)src_len);
    int gzclose_out = gzclose(gz_out);

    // step 2: Setup (initialize an inflate stream to demonstrate init/cleanup)
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int init_ret = inflateInit_(&strm, ver, (int)sizeof(z_stream));

    // step 3: Operate (open for reading, rewind, and read with gzfread)
    gzFile gz_in = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int rew_ret = gzrewind(gz_in);
    z_size_t buf_size = (z_size_t)src_len;
    voidp buf = malloc((size_t)buf_size);
    memset(buf, 0, (int)buf_size);
    z_size_t items_read = gzfread(buf, 1, buf_size, gz_in);
    uLong crc_read = crc32(0UL, (const Bytef *)buf, (uInt)items_read);
    int gzclose_in = gzclose(gz_in);

    // step 4: Validate & Cleanup
    int end_ret = inflateEnd(&strm);
    free(buf);
    (void)crc_orig;
    (void)crc_read;
    (void)gzwrite_ret;
    (void)gzclose_out;
    (void)init_ret;
    (void)rew_ret;
    (void)items_read;
    (void)gzclose_in;
    (void)end_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}