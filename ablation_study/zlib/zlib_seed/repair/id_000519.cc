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
//<ID> 519
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef plain_in[256];
    memset(plain_in, 'C', sizeof(plain_in));
    uLong adler_initial = adler32(0UL, plain_in, (uInt)sizeof(plain_in));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    int def_prime_ret = deflatePrime(&def_strm, 3, 5);
    Bytef comp_buf[1024];
    memset(comp_buf, 0, sizeof(comp_buf));

    // step 2: Configure
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)sizeof(plain_in);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong compressed_len = def_strm.total_out;
    const z_crc_t *crc_table = get_crc_table();
    uLong crc_comp = crc32(0UL, comp_buf, (uInt)compressed_len);
    uLong mixed_crc = crc_comp ^ (uLong)crc_table[0];

    // step 3: Operate
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gw = gzopen(filename, "wb");
    int gw_write = gzwrite(gw, comp_buf, (unsigned int)compressed_len);
    off_t gw_offset_after_write = gzoffset(gw);
    int gw_close = gzclose(gw);
    gzFile gr = gzopen(filename, "rb");
    z_stream inf_src;
    memset(&inf_src, 0, sizeof(inf_src));
    int inf_init_ret = inflateInit_(&inf_src, ver, (int)sizeof(z_stream));
    inf_src.next_in = comp_buf;
    inf_src.avail_in = (uInt)compressed_len;
    Bytef decomp_buf[512];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    inf_src.next_out = decomp_buf;
    inf_src.avail_out = (uInt)sizeof(decomp_buf);
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    int copy_ret = inflateCopy(&inf_copy, &inf_src);
    int inf_ret = inflate(&inf_copy, 4); /* 4 == Z_FINISH */
    uLong decompressed_len = inf_copy.total_out;
    uLong adler_from_decompressed = adler32(0UL, decomp_buf, (uInt)decompressed_len);
    uLong combined_adler = adler32_combine64(adler_initial, adler_from_decompressed, (off64_t)decompressed_len);
    off_t gr_offset = gzoffset(gr);
    int gr_read = gzread(gr, comp_buf, (unsigned int)sizeof(comp_buf));

    // step 4: Validate / Cleanup
    int def_end = deflateEnd(&def_strm);
    int inf_end_src = inflateEnd(&inf_src);
    int inf_end_copy = inflateEnd(&inf_copy);
    int gr_close = gzclose(gr);

    (void)ver;
    (void)def_init_ret;
    (void)def_prime_ret;
    (void)def_ret;
    (void)compressed_len;
    (void)crc_table;
    (void)mixed_crc;
    (void)gw_write;
    (void)gw_offset_after_write;
    (void)gw_close;
    (void)inf_init_ret;
    (void)copy_ret;
    (void)inf_ret;
    (void)decompressed_len;
    (void)adler_from_decompressed;
    (void)combined_adler;
    (void)gr_offset;
    (void)gr_read;
    (void)def_end;
    (void)inf_end_src;
    (void)inf_end_copy;
    (void)gr_close;
    // API sequence test completed successfully
    return 66;
}