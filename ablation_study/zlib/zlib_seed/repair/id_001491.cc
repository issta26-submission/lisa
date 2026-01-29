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
//<ID> 1491
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence: initialize deflate, set dictionary, compress data";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    const char * version = zlibVersion();
    int dinit_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    const Bytef dict[] = "sample_dict";
    int dsetdict_ret = deflateSetDictionary(&dstrm, dict, (uInt)(sizeof(dict) - 1));
    uLong bound = deflateBound(&dstrm, src_len);
    Bytef * comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    dstrm.next_in = (Bytef *)src;
    dstrm.avail_in = (uInt)src_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(((bound) > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)bound);
    int deflate_ret = deflate(&dstrm, 4); // Z_FINISH == 4
    uLong comp_len = dstrm.total_out;
    int deflate_end_ret = deflateEnd(&dstrm);

    // step 2: Configure
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gzset_ret = gzsetparams(gw, 6, 0);
    int gw_write_ret = gzwrite(gw, (voidp)comp_buf, (unsigned int)((comp_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int gw_close_ret = gzclose(gw);

    // step 3: Operate
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    z_size_t read_buf_size = (z_size_t)((comp_len + 16UL) > 0xFFFFFFFFUL ? 0xFFFFFFFFUL : (unsigned int)(comp_len + 16UL));
    Bytef * read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (int)read_buf_size);
    z_size_t items_read = gzfread(read_buf, 1, read_buf_size, gr);
    int gr_close_ret = gzclose(gr);

    // step 4: Validate & Cleanup
    free(comp_buf);
    free(read_buf);
    (void)dinit_ret;
    (void)version;
    (void)dsetdict_ret;
    (void)bound;
    (void)deflate_ret;
    (void)comp_len;
    (void)deflate_end_ret;
    (void)gzset_ret;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)items_read;
    (void)gr_close_ret;
    // API sequence test completed successfully
    return 66;
}