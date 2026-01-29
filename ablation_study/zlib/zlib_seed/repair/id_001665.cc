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
//<ID> 1665
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate -> write gz -> reopen gz -> read first byte";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;

    // step 2: Configure output buffer
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;

    // step 3: Operate (compress) and prepare inflate stream for a sync check
    int def_ret = deflate(&def_strm, 4);
    uLong out_len = def_strm.total_out;
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)out_len;
    int sync_ret = inflateSyncPoint(&inf_strm);

    // step 4: Validate & Cleanup (write compressed data to .gz, then read first byte back, cleanup)
    gzFile gzout = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzout, comp_buf, (unsigned int)out_len);
    int gf_ret = gzflush(gzout, 2);
    int gzclose_ret1 = gzclose(gzout);
    gzFile gzin = gzopen("test_zlib_api_sequence.gz", "rb");
    int first_byte = gzgetc_(gzin);
    int gzclose_ret2 = gzclose(gzin);
    int def_end = deflateEnd(&def_strm);
    int inf_end = inflateEnd(&inf_strm);
    free(comp_buf);
    (void)def_init_ret;
    (void)def_ret;
    (void)out_len;
    (void)inf_init_ret;
    (void)sync_ret;
    (void)gw_ret;
    (void)gf_ret;
    (void)gzclose_ret1;
    (void)first_byte;
    (void)gzclose_ret2;
    (void)def_end;
    (void)inf_end;
    (void)comp_bound;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}