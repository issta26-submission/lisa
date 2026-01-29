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
//<ID> 511
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef inbuf[256];
    memset(inbuf, 'A', sizeof(inbuf));
    uLong crc1 = crc32(0UL, inbuf, (uInt)sizeof(inbuf));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int dinit = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    int dprime = deflatePrime(&def_strm, 3, 5);
    Bytef compbuf[1024];
    memset(compbuf, 0, sizeof(compbuf));
    def_strm.next_in = inbuf;
    def_strm.avail_in = (uInt)sizeof(inbuf);
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);

    // step 2: Operate (compress) and checksum operations
    int ddef = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong produced = def_strm.total_out;
    uLong crc2 = crc32(0UL, compbuf, (uInt)produced);
    uLong combined_adler = adler32_combine64(crc1, crc2, (off64_t)produced);
    const z_crc_t *crc_table = get_crc_table();
    uLong mixed = (uLong)crc_table[0] ^ crc2;

    // step 3: File IO and inflate copy
    const char *fname = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(fname, "wb");
    int gw = gzwrite(gzw, compbuf, (unsigned int)produced);
    off_t off_written = gzoffset(gzw);
    int gzw_close = gzclose_w(gzw);
    gzFile gzr = gzopen(fname, "rb");
    off_t off_read_start = gzoffset(gzr);
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inits = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)produced;
    Bytef outbuf[512];
    memset(outbuf, 0, sizeof(outbuf));
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    int copy_ret = inflateCopy(&inf_copy, &inf_strm);
    int infl = inflate(&inf_strm, 4);
    uLong decompressed = inf_strm.total_out;
    int gr_close = gzclose_r(gzr);

    // step 4: Validate / Cleanup
    int dEnd = deflateEnd(&def_strm);
    int iEnd = inflateEnd(&inf_strm);
    int iCopyEnd = inflateEnd(&inf_copy);
    (void)ver;
    (void)dinit;
    (void)dprime;
    (void)ddef;
    (void)produced;
    (void)crc1;
    (void)crc2;
    (void)combined_adler;
    (void)crc_table;
    (void)mixed;
    (void)fname;
    (void)gw;
    (void)off_written;
    (void)gzw_close;
    (void)off_read_start;
    (void)inits;
    (void)decompressed;
    (void)gr_close;
    (void)dEnd;
    (void)iEnd;
    (void)iCopyEnd;
    (void)infl;
    (void)copy_ret;
    // API sequence test completed successfully
    return 66;
}