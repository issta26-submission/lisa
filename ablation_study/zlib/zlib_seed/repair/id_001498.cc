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
//<ID> 1498
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source, dictionary, and deflate stream)
    const Bytef src[] = "zlib API sequence: deflate with dictionary, write gz, read with gzfread";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const Bytef dict[] = "preset_dictionary";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char * ver = zlibVersion();
    int init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&strm, src_len);
    Bytef * outbuf = (Bytef *)malloc((size_t)bound);
    memset(outbuf, 0, (int)bound);

    // step 2: Configure (set dictionary for deflate and perform compression)
    int dict_ret = deflateSetDictionary(&strm, dict, dict_len);
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    strm.next_out = outbuf;
    strm.avail_out = (uInt)bound;
    int def_ret = deflate(&strm, 4); // 4 == Z_FINISH
    int def_end_ret = deflateEnd(&strm);

    // step 3: Operate (write original data to gz with gzsetparams, then read back with gzfread)
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int setparams_ret = gzsetparams(gw, 1, 0);
    int gw_write_ret = gzwrite(gw, (voidp)src, (unsigned int)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gw_close_ret = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    z_size_t read_buf_size = (z_size_t)(bound + 1UL);
    Bytef * read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (int)read_buf_size);
    z_size_t items_read = gzfread((voidp)read_buf, (z_size_t)1, read_buf_size, gr);
    int gr_close_ret = gzclose(gr);

    // step 4: Validate & Cleanup
    free(outbuf);
    free(read_buf);
    (void)init_ret;
    (void)ver;
    (void)bound;
    (void)dict_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)setparams_ret;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)items_read;
    (void)gr_close_ret;
    // API sequence test completed successfully
    return 66;
}