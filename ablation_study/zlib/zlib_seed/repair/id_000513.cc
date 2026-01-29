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
//<ID> 513
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef plain[128];
    memset(plain, 'A', sizeof(plain));
    uLong ad1 = adler32(0UL, plain, (uInt)sizeof(plain));
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    int prime_ret = deflatePrime(&def, 3, 5);
    def.next_in = plain;
    def.avail_in = (uInt)sizeof(plain);
    Bytef compbuf[512];
    memset(compbuf, 0, sizeof(compbuf));
    def.next_out = compbuf;
    def.avail_out = (uInt)sizeof(compbuf);

    // step 2: Operate (compress) and write to .gz
    int def_ret = deflate(&def, 4); /* 4 == Z_FINISH */
    uLong produced = def.total_out;
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(filename, "wb");
    int gw_ret = gzwrite(gzw, compbuf, (unsigned int)produced);
    int gw_close_ret = gzclose(gzw);

    // step 3: Read compressed file, compute checksums, copy inflate state and use CRC table
    gzFile gzr = gzopen(filename, "rb");
    Bytef readbuf[512];
    memset(readbuf, 0, sizeof(readbuf));
    int read_len = gzread(gzr, readbuf, (unsigned int)sizeof(readbuf));
    off_t off_after = gzoffset(gzr);
    uLong ad2 = adler32(0UL, readbuf, (uInt)read_len);
    uLong combined_ad = adler32_combine64(ad1, ad2, (off64_t)read_len);
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = compbuf;
    inf.avail_in = (uInt)produced;
    Bytef outbuf[256];
    memset(outbuf, 0, sizeof(outbuf));
    inf.next_out = outbuf;
    inf.avail_out = (uInt)sizeof(outbuf);
    int inf_ret = inflate(&inf, 0);
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    int copy_ret = inflateCopy(&inf_copy, &inf);
    const z_crc_t *crc_table = get_crc_table();
    uLong crc_val = crc32(0UL, readbuf, (uInt)read_len);
    uLong mixed = crc_val ^ (uLong)crc_table[0];

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def);
    int inf_end_ret = inflateEnd(&inf);
    int inf_copy_end_ret = inflateEnd(&inf_copy);
    int gz_close_ret = gzclose(gzr);

    (void)ver;
    (void)ad1;
    (void)def_init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)produced;
    (void)filename;
    (void)gw_ret;
    (void)gw_close_ret;
    (void)read_len;
    (void)off_after;
    (void)ad2;
    (void)combined_ad;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)copy_ret;
    (void)crc_table;
    (void)crc_val;
    (void)mixed;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)inf_copy_end_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}