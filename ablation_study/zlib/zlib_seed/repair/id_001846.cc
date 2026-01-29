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
//<ID> 1846
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare data and write to a gz file)
    const Bytef src[] = "zlib API sequence test: write -> rewind -> read -> crc -> inflateInit";
    uLong src_len = (uLong)(sizeof(src) - 1);
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gzwrite_ret = gzwrite(gw, src, (unsigned int)src_len);
    int gzclose_write_ret = gzclose(gw);

    // step 2: Setup (initialize inflate stream)
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int init_ret = inflateInit_(&strm, ver, (int)sizeof(z_stream));

    // step 3: Operate (open for read, rewind, read data with gzfread, compute crc, attach to stream)
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int rewind_ret = gzrewind(gr);
    Bytef *read_buf = (Bytef *)malloc((size_t)256);
    memset(read_buf, 0, (int)256);
    z_size_t items_read = gzfread(read_buf, (z_size_t)1, (z_size_t)256, gr);
    uInt bytes_read = (uInt)items_read;
    uLong data_crc = crc32(0UL, read_buf, bytes_read);
    strm.next_in = read_buf;
    strm.avail_in = bytes_read;
    strm.total_in = (uLong)bytes_read;

    // step 4: Validate & Cleanup
    int inflate_end_ret = inflateEnd(&strm);
    int gzclose_read_ret = gzclose(gr);
    free(read_buf);
    (void)gzwrite_ret;
    (void)gzclose_write_ret;
    (void)init_ret;
    (void)rewind_ret;
    (void)items_read;
    (void)data_crc;
    (void)inflate_end_ret;
    (void)gzclose_read_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}