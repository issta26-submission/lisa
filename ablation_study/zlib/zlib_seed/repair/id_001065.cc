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
//<ID> 1065
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence payload: test inflateInit_, inflateUndermine, gzwrite, gzseek and crc32_combine.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong crc1 = crc32(0UL, source, (uInt)source_len);

    // step 2: Configure
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;
    int init_ret = inflateInit_(&strm, version, (int)sizeof(z_stream));
    int undermine_ret = inflateUndermine(&strm, 1);

    // step 3: Operate
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gw, source, (unsigned int)source_len);
    off_t seek_ret = gzseek(gw, (off_t)0, 1);
    int gzclose_w = gzclose(gw);

    // step 4: Validate / Cleanup
    gzFile gr = gzopen("tmp_zlib_api_seq.gz", "rb");
    z_size_t read_buf_len = (z_size_t)(source_len + 64);
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_len);
    memset(read_buf, 0, (int)read_buf_len);
    z_size_t read_items = gzfread(read_buf, 1, read_buf_len, gr);
    int gzclose_r = gzclose(gr);
    uLong crc2 = crc32(0UL, read_buf, (uInt)read_items);
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)read_items);
    int infl_end = inflateEnd(&strm);
    free(read_buf);
    (void)init_ret;
    (void)undermine_ret;
    (void)gzwrite_ret;
    (void)seek_ret;
    (void)gzclose_w;
    (void)read_items;
    (void)gzclose_r;
    (void)combined_crc;
    (void)crc1;
    (void)crc2;
    (void)infl_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}