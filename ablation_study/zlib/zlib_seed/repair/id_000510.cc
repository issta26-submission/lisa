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
//<ID> 510
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
    memset(input, 'Z', sizeof(input));
    uLong crc_input = crc32(0UL, input, (uInt)sizeof(input));
    const z_crc_t *crc_tbl = get_crc_table();

    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    int prime_ret = deflatePrime(&def, 3, 5);

    z_stream inf_src;
    memset(&inf_src, 0, sizeof(inf_src));
    int inf_src_init = inflateInit_(&inf_src, ver, (int)sizeof(z_stream));
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    int inf_copy_ret = inflateCopy(&inf_copy, &inf_src);

    // step 2: Configure
    Bytef compbuf[1024];
    memset(compbuf, 0, sizeof(compbuf));
    def.next_in = input;
    def.avail_in = (uInt)sizeof(input);
    def.next_out = compbuf;
    def.avail_out = (uInt)sizeof(compbuf);

    // step 3: Operate
    int def_ret = deflate(&def, 4); /* 4 == Z_FINISH */
    uLong produced = def.total_out;
    uLong mix_crc = crc_input ^ (uLong)crc_tbl[0];

    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(filename, "wb");
    int gw_ret = gzwrite(gzw, compbuf, (unsigned int)produced);
    int gzclose_w_ret = gzclose(gzw);

    gzFile gzr = gzopen(filename, "rb");
    off_t off_before = gzoffset(gzr);
    Bytef readbuf[1024];
    memset(readbuf, 0, sizeof(readbuf));
    int read_len = gzread(gzr, readbuf, (unsigned int)sizeof(readbuf));
    uLong crc_read = adler32(0UL, readbuf, (uInt)read_len);
    uLong combined_adler = adler32_combine64(crc_input, crc_read, (off64_t)read_len);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def);
    int inf_src_end = inflateEnd(&inf_src);
    int inf_copy_end = inflateEnd(&inf_copy);
    int gzclose_r_ret = gzclose(gzr);

    (void)ver;
    (void)crc_input;
    (void)crc_tbl;
    (void)def_init_ret;
    (void)prime_ret;
    (void)inf_src_init;
    (void)inf_copy_ret;
    (void)def_ret;
    (void)produced;
    (void)mix_crc;
    (void)filename;
    (void)gw_ret;
    (void)gzclose_w_ret;
    (void)off_before;
    (void)read_len;
    (void)crc_read;
    (void)combined_adler;
    (void)def_end_ret;
    (void)inf_src_end;
    (void)inf_copy_end;
    (void)gzclose_r_ret;
    // API sequence test completed successfully
    return 66;
}