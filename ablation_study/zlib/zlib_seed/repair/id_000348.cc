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
//<ID> 348
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver = zlibVersion();
    z_stream inf_strm;
    Bytef src_buf[] = "Zlib API sequence payload for compress -> inflate -> gz operations.";
    uLong src_len = (uLong)(sizeof(src_buf) - 1);
    Bytef comp_buf[16384];
    Bytef decomp_buf[16384];
    uLongf comp_len_f;
    uLong decomp_len = 0;
    int comp_ret;
    int inf_init_ret;
    int inf_ret;
    int inf_end_ret;
    gzFile gz = (gzFile)0;
    int gzbuf_ret;
    int gzwrite_ret;
    int gzeof_ret;
    int gzcloser_ret;

    // step 2: Setup / initialization
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(comp_buf, 0, sizeof(comp_buf));
    memset(decomp_buf, 0, sizeof(decomp_buf));
    comp_len_f = (uLongf)sizeof(comp_buf);
    comp_ret = compress(comp_buf, &comp_len_f, (const Bytef *)src_buf, (uLong)src_len);

    // step 3: Core operations - initialize inflate, perform inflate, write compressed data to gz
    inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len_f;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);
    inf_ret = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzbuf_ret = gzbuffer(gz, 8192);
    gzwrite_ret = gzwrite(gz, (voidpc)comp_buf, (unsigned int)comp_len_f);
    gzeof_ret = gzeof(gz);

    // step 4: Cleanup / validation
    inf_end_ret = inflateEnd(&inf_strm);
    gzcloser_ret = gzclose_r(gz);
    (void)ver;
    (void)src_len;
    (void)comp_len_f;
    (void)decomp_len;
    (void)comp_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)gzbuf_ret;
    (void)gzwrite_ret;
    (void)gzeof_ret;
    (void)gzcloser_ret;
    // API sequence test completed successfully
    return 66;
}