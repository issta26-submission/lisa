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
//<ID> 481
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *ver = zlibVersion();
    Bytef input[256];
    memset(input, 'A', sizeof(input));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    int tune_ret = deflateTune(&def_strm, 32, 64, 128, 4096);
    Bytef dictionary[32];
    memset(dictionary, 0x5A, sizeof(dictionary));
    int dict_ret = deflateSetDictionary(&def_strm, dictionary, (uInt)sizeof(dictionary));

    // step 2: Configure / Operate (deflate)
    def_strm.next_in = input;
    def_strm.avail_in = (uInt)sizeof(input);
    Bytef compbuf[1024];
    memset(compbuf, 0, sizeof(compbuf));
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong produced = def_strm.total_out;

    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gzw, input, (unsigned int)sizeof(input));
    int gzclose_w_ret = gzclose(gzw);

    // step 3: Use gz read API and inflate the raw deflate output
    gzFile gzr = gzopen(filename, "rb");
    char readback[128];
    memset(readback, 0, sizeof(readback));
    int gzread_ret = gzread(gzr, readback, (unsigned int)sizeof(readback));
    int gzclose_r_ret = gzclose_r(gzr);

    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)produced;
    Bytef decomp[512];
    memset(decomp, 0, sizeof(decomp));
    inf_strm.next_out = decomp;
    inf_strm.avail_out = (uInt)sizeof(decomp);
    int inf_ret = inflate(&inf_strm, 4); /* 4 == Z_FINISH */
    int inf_end_ret = inflateEnd(&inf_strm);

    // step 4: Cleanup
    int def_end_ret = deflateEnd(&def_strm);

    (void)ver;
    (void)def_init_ret;
    (void)tune_ret;
    (void)dict_ret;
    (void)def_ret;
    (void)produced;
    (void)filename;
    (void)gzw;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)gzr;
    (void)gzread_ret;
    (void)gzclose_r_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}