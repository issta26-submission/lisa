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
//<ID> 499
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
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    int prime_ret = deflatePrime(&def_strm, 3, 5);
    Bytef compbuf[1024];
    memset(compbuf, 0, sizeof(compbuf));
    def_strm.next_in = input;
    def_strm.avail_in = (uInt)sizeof(input);
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);

    // step 2: Operate (compress) and compute checksum using CRC table
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong produced = def_strm.total_out;
    const z_crc_t *crc_table = get_crc_table();
    uLong checksum = crc32(0UL, compbuf, (uInt)produced);
    uLong mixed = checksum ^ (uLong)crc_table[0];

    // step 3: Write compressed data to .gz, read one byte using gzgetc_ and push back with gzungetc
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gzw, compbuf, (unsigned int)produced);
    int gzclose_w_ret = gzclose(gzw);
    gzFile gzr = gzopen(filename, "rb");
    int ch1 = gzgetc_(gzr);
    int unget_ret = gzungetc(ch1, gzr);
    int ch2 = gzgetc_(gzr);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    int gzclose_r_ret = gzclose_r(gzr);

    (void)ver;
    (void)def_init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)produced;
    (void)crc_table;
    (void)checksum;
    (void)mixed;
    (void)filename;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)ch1;
    (void)unget_ret;
    (void)ch2;
    (void)def_end_ret;
    (void)gzclose_r_ret;
    // API sequence test completed successfully
    return 66;
}