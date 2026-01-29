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
//<ID> 493
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef input[256];
    memset(input, 'A', sizeof(input));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    int prime_ret = deflatePrime(&def_strm, 3, 5);

    // step 2: Configure
    def_strm.next_in = input;
    def_strm.avail_in = (uInt)sizeof(input);
    Bytef compbuf[1024];
    memset(compbuf, 0, sizeof(compbuf));
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(filename, "wb");

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong produced = def_strm.total_out;
    int gzwrite_ret = gzwrite(gzw, compbuf, (unsigned int)produced);
    int gzclose_w_ret = gzclose_w(gzw);
    gzFile gzr = gzopen(filename, "rb");
    unsigned char read_buf[1024];
    memset(read_buf, 0, sizeof(read_buf));
    int first_char = gzgetc_(gzr);
    int unget_ret = gzungetc(first_char, gzr);
    int second_char = gzgetc_(gzr);
    int read_len = gzread(gzr, read_buf, (unsigned int)sizeof(read_buf));
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t crc0 = crc_tbl[0];

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    int gzclose_r_ret = gzclose_r(gzr);

    (void)ver;
    (void)def_init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)produced;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)first_char;
    (void)unget_ret;
    (void)second_char;
    (void)read_len;
    (void)crc_tbl;
    (void)crc0;
    (void)def_end_ret;
    (void)gzclose_r_ret;
    (void)filename;
    (void)input;
    (void)compbuf;
    (void)read_buf;
    // API sequence test completed successfully
    return 66;
}