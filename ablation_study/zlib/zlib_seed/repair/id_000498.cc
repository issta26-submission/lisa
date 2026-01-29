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
//<ID> 498
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
    memset(input, 'A', sizeof(input));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    int prime_ret = deflatePrime(&def_strm, 3, 5);
    const z_crc_t *crc_tbl = get_crc_table();

    // step 2: Configure
    def_strm.next_in = input;
    def_strm.avail_in = (uInt)sizeof(input);
    Bytef compbuf[512];
    memset(compbuf, 0, sizeof(compbuf));
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong produced = def_strm.total_out;
    uLong checksum = ((uLong)crc_tbl[0]) ^ produced;
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gzw, compbuf, (unsigned int)produced);
    int gzclose_w_ret = gzclose(gzw);
    gzFile gzr = gzopen(filename, "rb");
    int c_read1 = gzgetc_(gzr);
    int ungetc_ret = gzungetc(c_read1, gzr);
    int c_read2 = gzgetc_(gzr);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    int gzclose_r_ret = gzclose_r(gzr);

    (void)ver;
    (void)def_init_ret;
    (void)prime_ret;
    (void)crc_tbl;
    (void)def_ret;
    (void)produced;
    (void)checksum;
    (void)filename;
    (void)gzw;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)c_read1;
    (void)ungetc_ret;
    (void)c_read2;
    (void)def_end_ret;
    (void)gzclose_r_ret;
    // API sequence test completed successfully
    return 66;
}