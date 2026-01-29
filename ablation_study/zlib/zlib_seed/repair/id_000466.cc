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
//<ID> 466
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    const Bytef src[] = "Hello zlib!\n"; // source data
    const uInt src_len = (uInt)(sizeof(src) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int deflate_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    Bytef compbuf[4096];
    memset(compbuf, 0, sizeof(compbuf));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = src_len;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);

    // step 2: Operate (deflate) and inspect pending bits, then reset deflater
    int deflate_ret = deflate(&def_strm, 4); // 4 == Z_FINISH
    uLong produced = def_strm.total_out;
    unsigned int pending_bits_count = 0;
    int pending_bits_value = 0;
    int deflate_pending_ret = deflatePending(&def_strm, &pending_bits_count, &pending_bits_value);
    int deflate_reset_ret = deflateReset(&def_strm);

    // step 3: Inflate the produced compressed data and inspect codes used
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inflate_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)produced;
    Bytef recover[4096];
    memset(recover, 0, sizeof(recover));
    inf_strm.next_out = recover;
    inf_strm.avail_out = (uInt)sizeof(recover);
    int inflate_ret = inflate(&inf_strm, 4); // 4 == Z_FINISH
    unsigned long codes_used = inflateCodesUsed(&inf_strm);

    // step 4: Write recovered data to a gzip file, read a line back with gzgets, and cleanup
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gzw, recover, (unsigned int)inf_strm.total_out);
    int gzclose_w_ret = gzclose(gzw);
    gzFile gzr = gzopen(filename, "rb");
    char linebuf[256];
    memset(linebuf, 0, sizeof(linebuf));
    char *gzgets_ret = gzgets(gzr, linebuf, (int)sizeof(linebuf));
    int gzclose_r_ret = gzclose(gzr);

    int deflate_end_ret = deflateEnd(&def_strm);
    int inflate_end_ret = inflateEnd(&inf_strm);

    (void)ver;
    (void)deflate_init_ret;
    (void)deflate_ret;
    (void)produced;
    (void)deflate_pending_ret;
    (void)pending_bits_count;
    (void)pending_bits_value;
    (void)deflate_reset_ret;
    (void)inflate_init_ret;
    (void)inflate_ret;
    (void)codes_used;
    (void)filename;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)gzgets_ret;
    (void)gzclose_r_ret;
    (void)deflate_end_ret;
    (void)inflate_end_ret;
    // API sequence test completed successfully
    return 66;
}