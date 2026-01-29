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
//<ID> 1494
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate with dictionary -> write gz with params -> read with gzfread";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char * version = zlibVersion();
    int def_init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    const Bytef dict[] = "zlib API"; 
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    int dict_ret = deflateSetDictionary(&strm, dict, dict_len);
    uLong bound = deflateBound(&strm, src_len);
    Bytef * comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);

    // step 2: Configure (compress using deflate, finalize, then write compressed bytes into a .gz with params)
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int def_ret = deflate(&strm, 4); // Z_FINISH == 4
    uLong comp_size = strm.total_out;
    int def_end_ret = deflateEnd(&strm);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gzset_ret = gzsetparams(gw, 6, 0);
    int gw_write_ret = gzwrite(gw, (voidp)comp_buf, (unsigned int)((comp_size > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)comp_size));
    int gw_close_ret = gzclose(gw);

    // step 3: Operate (open for read and use gzfread to ingest the written data)
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    z_size_t read_buf_size = (z_size_t)((bound > 4096UL) ? bound : 4096UL);
    Bytef * read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (int)read_buf_size);
    z_size_t items_read = gzfread((voidp)read_buf, (z_size_t)1, read_buf_size, gr);

    // step 4: Validate & Cleanup
    int gr_close_ret = gzclose(gr);
    free(comp_buf);
    free(read_buf);
    (void)def_init_ret;
    (void)version;
    (void)dict_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_size;
    (void)def_end_ret;
    (void)gzset_ret;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)items_read;
    (void)gr_close_ret;
    // API sequence test completed successfully
    return 66;
}