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
//<ID> 502
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef input[128];
    memset(input, 'A', sizeof(input));
    uInt half = (uInt)(sizeof(input) / 2);
    uLong ad1 = adler32(1UL, input, half);
    uLong ad2 = adler32(1UL, input + half, half);

    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure (prepare compression buffers and perform compression)
    Bytef compbuf[512];
    memset(compbuf, 0, sizeof(compbuf));
    def_strm.next_in = input;
    def_strm.avail_in = (uInt)sizeof(input);
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong produced = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Operate (checksums and write compressed data to .gz)
    uLong crc_comp = crc32(0UL, compbuf, (uInt)produced);
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gzw, compbuf, (unsigned int)produced);
    int gzclose_ret = gzclose(gzw);

    // step 4: Validate / Cleanup (inflate compressed data and combine adler checksums)
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)produced;
    Bytef decomp[256];
    memset(decomp, 0, sizeof(decomp));
    inf_strm.next_out = decomp;
    inf_strm.avail_out = (uInt)sizeof(decomp);
    int inf_ret = inflate(&inf_strm, 4); /* 4 == Z_FINISH */
    uLong decomp_produced = inf_strm.total_out;
    int inf_end_ret = inflateEnd(&inf_strm);

    uLong combined_adler = adler32_combine64(ad1, ad2, (off64_t)half);

    (void)ver;
    (void)def_init_ret;
    (void)def_ret;
    (void)produced;
    (void)def_end_ret;
    (void)crc_comp;
    (void)filename;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)decomp_produced;
    (void)inf_end_ret;
    (void)combined_adler;
    // API sequence test completed successfully
    return 66;
}