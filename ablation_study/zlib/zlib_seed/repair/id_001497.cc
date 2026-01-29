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
//<ID> 1497
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib sequence test: deflate with dictionary -> write gz -> read gz via gzfread -> cleanup";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const Bytef dict[] = "example-dictionary";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char * version = zlibVersion();
    int def_init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    int def_setdict_ret = deflateSetDictionary(&strm, dict, dict_len);

    // step 2: Configure (prepare buffers and perform a deflate call)
    uLong bound = deflateBound(&strm, src_len);
    Bytef * comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)((bound > (uLong)0xFFFFFFFFU) ? 0xFFFFFFFFU : (uInt)bound);
    int def_ret = deflate(&strm, 0);
    uLong comp_size = strm.total_out;
    int def_end_ret = deflateEnd(&strm);

    // step 3: Operate (write compressed output to a gzip file, adjust params, then read it back with gzfread)
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_write_ret = gzwrite(gw, (voidp)comp_buf, (unsigned int)((comp_size > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)comp_size));
    int gz_setparams_ret = gzsetparams(gw, 6, 0);
    int gw_close_ret = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef * read_buf = (Bytef *)malloc((size_t)(bound + 16UL));
    memset(read_buf, 0, (int)(bound + 16UL));
    z_size_t items_read = gzfread(read_buf, (z_size_t)1, (z_size_t)(bound + 16UL), gr);

    // step 4: Validate & Cleanup
    int gr_close_ret = gzclose(gr);
    free(comp_buf);
    free(read_buf);
    (void)def_init_ret;
    (void)def_setdict_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)bound;
    (void)comp_size;
    (void)gw_write_ret;
    (void)gz_setparams_ret;
    (void)gw_close_ret;
    (void)items_read;
    (void)gr_close_ret;
    // API sequence test completed successfully
    return 66;
}