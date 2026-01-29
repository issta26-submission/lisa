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
//<ID> 515
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
    uLong adler_in = adler32(0UL, input, (uInt)sizeof(input));
    z_stream def;
    z_stream inf;
    z_stream inf_copy;
    memset(&def, 0, sizeof(def));
    memset(&inf, 0, sizeof(inf));
    memset(&inf_copy, 0, sizeof(inf_copy));
    int def_init_ret = deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    int inf_init_ret = inflateInit_(&inf, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef compbuf[1024];
    memset(compbuf, 0, sizeof(compbuf));
    def.next_in = input;
    def.avail_in = (uInt)sizeof(input);
    def.next_out = compbuf;
    def.avail_out = (uInt)sizeof(compbuf);
    int prime_ret = deflatePrime(&def, 3, 5);

    // step 3: Operate
    int def_ret = deflate(&def, 4); /* 4 == Z_FINISH */
    uLong produced = def.total_out;
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_wr = gzwrite(gw, compbuf, (unsigned int)produced);
    int gw_close = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    int read_len = gzread(gr, compbuf, (unsigned int)sizeof(compbuf));
    uLong adler_read = adler32(0UL, compbuf, (uInt)read_len);
    off_t current_offset = gzoffset(gr);
    int infl_copy_ret = inflateCopy(&inf_copy, &inf);
    const z_crc_t *crc_tbl = get_crc_table();
    uLong combined_adler = adler32_combine64(adler_in, adler_read, (off64_t)read_len);
    uLong mix_val = combined_adler ^ (uLong)crc_tbl[0];

    // step 4: Validate / Cleanup
    int inf_end = inflateEnd(&inf);
    int inf_copy_end = inflateEnd(&inf_copy);
    int def_end = deflateEnd(&def);
    int gr_close = gzclose_r(gr);

    (void)ver;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)produced;
    (void)gw_wr;
    (void)gw_close;
    (void)read_len;
    (void)adler_read;
    (void)current_offset;
    (void)infl_copy_ret;
    (void)crc_tbl;
    (void)combined_adler;
    (void)mix_val;
    (void)inf_end;
    (void)inf_copy_end;
    (void)def_end;
    (void)gr_close;
    // API sequence test completed successfully
    return 66;
}