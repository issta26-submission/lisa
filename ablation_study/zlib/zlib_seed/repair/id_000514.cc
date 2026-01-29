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
//<ID> 514
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef src[256];
    memset(src, 'B', sizeof(src));
    uLong ad_initial = adler32(0UL, src, (uInt)sizeof(src));
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    int prime_ret = deflatePrime(&def, 3, 5);

    // step 2: Configure buffers and streams
    Bytef compbuf[1024];
    memset(compbuf, 0, sizeof(compbuf));
    def.next_in = src;
    def.avail_in = (uInt)sizeof(src);
    def.next_out = compbuf;
    def.avail_out = (uInt)sizeof(compbuf);

    // step 3: Operate - compress, checksum, write/read gz, decompress copy
    int def_ret = deflate(&def, 4);
    uLong produced = def.total_out;
    const z_crc_t *crc_table = get_crc_table();
    uLong checksum = crc32(0UL, compbuf, (uInt)produced);
    uLong mixed = checksum ^ (uLong)crc_table[0];

    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gw = gzopen(filename, "wb");
    int gw_wr = gzwrite(gw, src, (unsigned int)sizeof(src));
    int gw_close = gzclose(gw);
    gzFile gr = gzopen(filename, "rb");
    Bytef readbuf[512];
    memset(readbuf, 0, sizeof(readbuf));
    int read_len = gzread(gr, readbuf, (unsigned int)sizeof(readbuf));
    off_t offset_after_read = gzoffset(gr);

    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = compbuf;
    inf.avail_in = (uInt)produced;
    Bytef outbuf[512];
    memset(outbuf, 0, sizeof(outbuf));
    inf.next_out = outbuf;
    inf.avail_out = (uInt)sizeof(outbuf);
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    int copy_ret = inflateCopy(&inf_copy, &inf);
    int inf_ret = inflate(&inf, 0);
    uLong ad_after = adler32(0UL, outbuf, (uInt)inf.total_out);
    uLong combined_ad = adler32_combine64(ad_initial, ad_after, (off64_t)inf.total_out);

    // step 4: Validate / Cleanup
    int def_end = deflateEnd(&def);
    int inf_end = inflateEnd(&inf);
    int inf_copy_end = inflateEnd(&inf_copy);
    int gr_close = gzclose(gr);

    (void)ver;
    (void)def_init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)produced;
    (void)crc_table;
    (void)checksum;
    (void)mixed;
    (void)filename;
    (void)gw_wr;
    (void)gw_close;
    (void)read_len;
    (void)offset_after_read;
    (void)inf_init_ret;
    (void)copy_ret;
    (void)inf_ret;
    (void)ad_after;
    (void)combined_ad;
    (void)def_end;
    (void)inf_end;
    (void)inf_copy_end;
    (void)gr_close;
    // API sequence test completed successfully
    return 66;
}