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
//<ID> 512
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
    uLong ad1 = adler32(0UL, input, (uInt)sizeof(input));
    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));
    int prime_ret = deflatePrime(&defstrm, 3, 5);
    Bytef compbuf[1024];
    memset(compbuf, 0, sizeof(compbuf));
    defstrm.next_in = input;
    defstrm.avail_in = (uInt)sizeof(input);
    defstrm.next_out = compbuf;
    defstrm.avail_out = (uInt)sizeof(compbuf);

    // step 2: Operate (compress) and prepare inflate streams
    int def_ret = deflate(&defstrm, 4); /* 4 == Z_FINISH */
    uLong produced = defstrm.total_out;
    z_stream inf_src;
    memset(&inf_src, 0, sizeof(inf_src));
    int inf_init_ret = inflateInit_(&inf_src, ver, (int)sizeof(z_stream));
    inf_src.next_in = compbuf;
    inf_src.avail_in = (uInt)produced;
    Bytef outbuf[512];
    memset(outbuf, 0, sizeof(outbuf));
    inf_src.next_out = outbuf;
    inf_src.avail_out = (uInt)sizeof(outbuf);
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    int copy_ret = inflateCopy(&inf_copy, &inf_src);

    // step 3: Checksums, gz IO and offset usage
    uLong crc_comp = crc32(0UL, compbuf, (uInt)produced);
    const z_crc_t *crc_table = get_crc_table();
    uLong mixed = crc_comp ^ (uLong)crc_table[0];
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gw = gzopen(filename, "wb");
    int gw_wr = gzwrite(gw, compbuf, (unsigned int)produced);
    off_t write_offset = gzoffset(gw);
    int gw_close_w = gzclose_w(gw);
    gzFile gr = gzopen(filename, "rb");
    off_t read_offset_before = gzoffset(gr);
    int read_len = gzread(gr, outbuf, (unsigned int)sizeof(outbuf));
    uLong ad2 = adler32(0UL, outbuf, (uInt)read_len);
    uLong combined = adler32_combine64(ad1, ad2, (off64_t)read_len);
    off_t read_offset_after = gzoffset(gr);
    int gr_close_r = gzclose_r(gr);

    // step 4: Validate / Cleanup
    int def_end = deflateEnd(&defstrm);
    int inf_end_src = inflateEnd(&inf_src);
    int inf_end_copy = inflateEnd(&inf_copy);

    (void)ver;
    (void)def_init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)produced;
    (void)inf_init_ret;
    (void)copy_ret;
    (void)crc_comp;
    (void)crc_table;
    (void)mixed;
    (void)filename;
    (void)gw_wr;
    (void)write_offset;
    (void)gw_close_w;
    (void)read_offset_before;
    (void)read_len;
    (void)ad2;
    (void)combined;
    (void)read_offset_after;
    (void)gr_close_r;
    (void)def_end;
    (void)inf_end_src;
    (void)inf_end_copy;
    // API sequence test completed successfully
    return 66;
}