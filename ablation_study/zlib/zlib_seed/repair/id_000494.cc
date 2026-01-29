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
//<ID> 494
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef input[128];
    memset(input, 'X', sizeof(input));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef compbuf[512];
    memset(compbuf, 0, sizeof(compbuf));
    def_strm.next_in = input;
    def_strm.avail_in = (uInt)sizeof(input);
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    int prime_ret = deflatePrime(&def_strm, 3, 5);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong produced = def_strm.total_out;
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gzw, compbuf, (unsigned int)produced);
    int gzclose_w_ret = gzclose_w(gzw);

    // step 4: Validate / Cleanup
    gzFile gzr = gzopen(filename, "rb");
    int first_ch = gzgetc_(gzr);
    int ungetc_ret = gzungetc(first_ch, gzr);
    int second_ch = gzgetc_(gzr);
    const z_crc_t *crc_tbl = get_crc_table();
    uLong combined = ((uLong)(crc_tbl ? crc_tbl[0] : 0)) ^ (uLong)second_ch;
    int gzclose_r_ret = gzclose_r(gzr);
    int def_end_ret = deflateEnd(&def_strm);

    (void)ver;
    (void)def_init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)produced;
    (void)filename;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)first_ch;
    (void)ungetc_ret;
    (void)second_ch;
    (void)crc_tbl;
    (void)combined;
    (void)gzclose_r_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}