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
//<ID> 516
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
    memset(input, 'I', sizeof(input));
    uLong init_adler = adler32(0UL, input, (uInt)sizeof(input));
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    int prime_ret = deflatePrime(&def, 3, 1);

    // step 2: Configure compression buffers and compress
    Bytef compbuf[1024];
    memset(compbuf, 0, sizeof(compbuf));
    def.next_in = input;
    def.avail_in = (uInt)sizeof(input);
    def.next_out = compbuf;
    def.avail_out = (uInt)sizeof(compbuf);
    int def_ret = deflate(&def, 4);
    uLong comp_produced = def.total_out;
    const z_crc_t *crc_tab = get_crc_table();
    uLong comp_crc = crc32(0UL, compbuf, (uInt)comp_produced);
    uLong mixed_crc = comp_crc ^ (uLong)crc_tab[0];

    // step 3: Write compressed data to .gz, read decompressed data, and use inflateCopy
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gw = gzopen(filename, "wb");
    int gw_write = gzwrite(gw, compbuf, (unsigned int)comp_produced);
    int gw_close = gzclose_w(gw);
    gzFile gr = gzopen(filename, "rb");
    off_t before_offset = gzoffset(gr);
    Bytef outbuf[512];
    memset(outbuf, 0, sizeof(outbuf));
    int read_len = gzread(gr, outbuf, (unsigned int)sizeof(outbuf));
    uLong post_adler = adler32(init_adler, outbuf, (uInt)read_len);
    uLong combined_adler = adler32_combine64(init_adler, post_adler, (off64_t)read_len);
    z_stream inf_src;
    memset(&inf_src, 0, sizeof(inf_src));
    int inf_init_ret = inflateInit_(&inf_src, ver, (int)sizeof(z_stream));
    inf_src.next_in = compbuf;
    inf_src.avail_in = (uInt)comp_produced;
    Bytef temp_out[512];
    memset(temp_out, 0, sizeof(temp_out));
    inf_src.next_out = temp_out;
    inf_src.avail_out = (uInt)sizeof(temp_out);
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    int copy_ret = inflateCopy(&inf_copy, &inf_src);
    int inf_ret = inflate(&inf_copy, 4);
    uLong inflated_total = inf_copy.total_out;

    // step 4: Validate / Cleanup
    int inf_end_src = inflateEnd(&inf_src);
    int inf_end_copy = inflateEnd(&inf_copy);
    int def_end = deflateEnd(&def);
    int gr_close = gzclose_r(gr);

    (void)ver;
    (void)init_adler;
    (void)def_init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)comp_produced;
    (void)crc_tab;
    (void)comp_crc;
    (void)mixed_crc;
    (void)filename;
    (void)gw_write;
    (void)gw_close;
    (void)before_offset;
    (void)read_len;
    (void)post_adler;
    (void)combined_adler;
    (void)inf_init_ret;
    (void)copy_ret;
    (void)inf_ret;
    (void)inflated_total;
    (void)inf_end_src;
    (void)inf_end_copy;
    (void)def_end;
    (void)gr_close;
    // API sequence test completed successfully
    return 66;
}