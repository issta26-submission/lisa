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
//<ID> 1841
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: write gz -> read gz -> inflateInit_ -> crc32 validate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    gzFile gz_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_written = gzwrite(gz_w, src, (unsigned int)src_len);
    int gzclose_w = gzclose(gz_w);

    // step 2: Setup
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int init_ret = inflateInit_(&strm, ver, (int)sizeof(z_stream));

    // step 3: Operate
    gzFile gz_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    z_size_t buf_size = (z_size_t)(src_len + 16U);
    voidp read_buf = malloc((size_t)buf_size);
    memset(read_buf, 0, (int)buf_size);
    z_size_t items_read1 = gzfread(read_buf, 1, buf_size, gz_r);
    int rewind_ret = gzrewind(gz_r);
    z_size_t items_read2 = gzfread(read_buf, 1, buf_size, gz_r);

    // step 4: Validate & Cleanup
    uLong crc_first = crc32(0UL, (const Bytef *)read_buf, (uInt)items_read1);
    uLong crc_combined = crc32(crc_first, (const Bytef *)read_buf, (uInt)items_read2);
    int gzclose_r = gzclose(gz_r);
    int inflate_end = inflateEnd(&strm);
    free(read_buf);
    (void)gz_written;
    (void)gzclose_w;
    (void)init_ret;
    (void)items_read1;
    (void)items_read2;
    (void)rewind_ret;
    (void)crc_first;
    (void)crc_combined;
    (void)gzclose_r;
    (void)inflate_end;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}