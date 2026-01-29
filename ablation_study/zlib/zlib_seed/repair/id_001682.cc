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
//<ID> 1682
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence: compute deflateBound -> deflate -> write gz -> reopen gz -> combine adler32";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;

    // step 2: Configure
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    gzFile gz_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz_w, comp_buf, (unsigned int)comp_len);
    int gflush_w = gzflush(gz_w, 2);
    int gzclose_w = gzclose(gz_w);
    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    int push_ret = gzungetc((int)'Z', gz_r);
    int first_char = gzgetc_(gz_r);
    int gzclose_r = gzclose(gz_r);

    // step 4: Validate & Cleanup
    uLong ad1 = adler32(1UL, src, (uInt)src_len);
    uLong ad2 = adler32(1UL, comp_buf, (uInt)comp_len);
    uLong combined = adler32_combine(ad1, ad2, (off_t)comp_len);
    int def_end = deflateEnd(&def_strm);
    free(comp_buf);
    (void)def_init_ret;
    (void)def_ret;
    (void)bound;
    (void)gw;
    (void)gflush_w;
    (void)gzclose_w;
    (void)push_ret;
    (void)first_char;
    (void)gzclose_r;
    (void)ad1;
    (void)ad2;
    (void)combined;
    (void)def_end;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}