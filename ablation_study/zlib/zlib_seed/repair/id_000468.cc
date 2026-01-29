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
//<ID> 468
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    z_stream in_strm;
    memset(&in_strm, 0, sizeof(in_strm));
    int inf_init_ret = inflateInit_(&in_strm, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef input_buf[1024];
    memset(input_buf, 'A', sizeof(input_buf));
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)sizeof(input_buf);
    Bytef out_buf[65536];
    memset(out_buf, 0, sizeof(out_buf));
    def_strm.next_out = out_buf;
    def_strm.avail_out = (uInt)sizeof(out_buf);
    int def_reset_ret = deflateReset(&def_strm);
    unsigned int pending_bits = 0;
    int bits_val = 0;
    int def_pending_ret = deflatePending(&def_strm, &pending_bits, &bits_val);

    // step 3: Operate
    int deflate_ret = deflate(&def_strm, 4); // Z_FINISH == 4
    uLong produced = def_strm.total_out;
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gzw, out_buf, (unsigned int)produced);
    int gzclose_w_ret = gzclose(gzw);
    gzFile gzr = gzopen(filename, "rb");
    char linebuf[256];
    memset(linebuf, 0, sizeof(linebuf));
    char *gzgets_ret = gzgets(gzr, linebuf, (int)sizeof(linebuf));
    int gzclose_r_ret = gzclose(gzr);

    // step 4: Validate / Cleanup
    unsigned long codes_used_before = inflateCodesUsed(&in_strm);
    int deflate_end_ret = deflateEnd(&def_strm);
    int inflate_end_ret = inflateEnd(&in_strm);

    (void)ver;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)def_reset_ret;
    (void)def_pending_ret;
    (void)pending_bits;
    (void)bits_val;
    (void)deflate_ret;
    (void)produced;
    (void)filename;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)gzgets_ret;
    (void)linebuf;
    (void)gzclose_r_ret;
    (void)codes_used_before;
    (void)deflate_end_ret;
    (void)inflate_end_ret;
    // API sequence test completed successfully
    return 66;
}