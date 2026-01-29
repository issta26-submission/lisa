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
//<ID> 487
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    int tune_ret = deflateTune(&def_strm, 32, 64, 128, 4096);
    Bytef dict[16];
    memset(dict, 0x42, sizeof(dict));
    int setdict_ret = deflateSetDictionary(&def_strm, dict, (uInt)sizeof(dict));
    Bytef input[256];
    memset(input, 'A', sizeof(input));
    def_strm.next_in = input;
    def_strm.avail_in = (uInt)sizeof(input);
    Bytef compbuf[512];
    memset(compbuf, 0, sizeof(compbuf));
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));

    // step 2: Configure
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(filename, "wb");

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong produced = def_strm.total_out;
    int gzwrite_ret = gzwrite(gzw, compbuf, (unsigned int)produced);
    int gzclose_w_ret = gzclose(gzw);
    gzFile gzr = gzopen(filename, "rb");
    char readbuf[512];
    memset(readbuf, 0, sizeof(readbuf));
    int gzread_ret = gzread(gzr, readbuf, (unsigned int)sizeof(readbuf));
    int gzclose_r_ret = gzclose_r(gzr);

    // step 4: Validate / Cleanup
    inf_strm.next_in = (Bytef *)readbuf;
    inf_strm.avail_in = (uInt)gzread_ret;
    Bytef decomp[256];
    memset(decomp, 0, sizeof(decomp));
    inf_strm.next_out = decomp;
    inf_strm.avail_out = (uInt)sizeof(decomp);
    int inf_ret = inflate(&inf_strm, 4); /* 4 == Z_FINISH */
    int inf_end_ret = inflateEnd(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);

    (void)ver;
    (void)def_init_ret;
    (void)tune_ret;
    (void)setdict_ret;
    (void)inf_init_ret;
    (void)filename;
    (void)def_ret;
    (void)produced;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)gzread_ret;
    (void)gzclose_r_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}