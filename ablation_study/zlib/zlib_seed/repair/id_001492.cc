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
//<ID> 1492
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare data, stream and buffers)
    const Bytef src[] = "zlib API sequence test: deflate with dictionary -> write gz -> read gz with gzfread -> checksum";
    const Bytef dict[] = "example_dict";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    uLong bound = deflateBound(&strm, src_len);
    Bytef * comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    int init_ret = deflateInit_(&strm, 6, zlibVersion(), (int)sizeof(z_stream));
    int dict_ret = deflateSetDictionary(&strm, (const Bytef *)dict, dict_len);

    // step 2: Configure (open gzip file for writing and set parameters)
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int setparams_ret = gzsetparams(gw, 6, 0);

    // step 3: Operate (compress with deflate and write compressed bytes to gzip file)
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int def_ret = deflate(&strm, 4); // 4 == Z_FINISH
    unsigned int comp_written = (unsigned int)((bound - (uLong)strm.avail_out) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)(bound - (uLong)strm.avail_out));
    int gw_write_ret = gzwrite(gw, (voidp)comp_buf, comp_written);
    int gw_close_ret = gzclose(gw);

    // step 4: Validate & Cleanup (read back with gzfread, checksum, and cleanup)
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    z_size_t read_size = (z_size_t)(bound + 16UL);
    Bytef * read_buf = (Bytef *)malloc((size_t)read_size);
    memset(read_buf, 0, (int)read_size);
    z_size_t items_read = gzfread((voidp)read_buf, (z_size_t)1, read_size, gr);
    uLong checksum = adler32(0UL, read_buf, (uInt)items_read);
    int gr_close_ret = gzclose(gr);
    int def_end_ret = deflateEnd(&strm);
    free(comp_buf);
    free(read_buf);
    (void)init_ret;
    (void)dict_ret;
    (void)setparams_ret;
    (void)def_ret;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)items_read;
    (void)checksum;
    (void)gr_close_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}