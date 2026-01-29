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
//<ID> 492
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
    memset(input, 'X', sizeof(input));
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int def_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&strm, (uLong)sizeof(input));

    // step 2: Configure
    strm.next_in = input;
    strm.avail_in = (uInt)sizeof(input);
    Bytef compbuf[1024];
    memset(compbuf, 0, sizeof(compbuf));
    strm.next_out = compbuf;
    strm.avail_out = (uInt)sizeof(compbuf);
    int prime_ret = deflatePrime(&strm, 3, 5);

    // step 3: Operate
    int def_ret = deflate(&strm, 4);
    uLong produced = strm.total_out;
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gzw, compbuf, (unsigned int)produced);
    int gzclose_w_ret = gzclose_w(gzw);

    // step 4: Validate / Cleanup
    gzFile gzr = gzopen(filename, "rb");
    int first_char = gzgetc_(gzr);
    int unget_ret = gzungetc(first_char, gzr);
    int second_char = gzgetc_(gzr);
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t sample_crc = crc_tbl[0];
    int def_end_ret = deflateEnd(&strm);
    int gzclose_r_ret = gzclose_r(gzr);

    (void)ver;
    (void)def_init_ret;
    (void)bound;
    (void)prime_ret;
    (void)def_ret;
    (void)produced;
    (void)filename;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)first_char;
    (void)unget_ret;
    (void)second_char;
    (void)sample_crc;
    (void)def_end_ret;
    (void)gzclose_r_ret;
    // API sequence test completed successfully
    return 66;
}