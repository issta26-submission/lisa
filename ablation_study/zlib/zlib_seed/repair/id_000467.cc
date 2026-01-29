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
//<ID> 467
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
    z_stream in_strm;
    memset(&in_strm, 0, sizeof(in_strm));
    int inf_init_ret = inflateInit_(&in_strm, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef input[128];
    memset(input, 'A', sizeof(input));
    def_strm.next_in = input;
    def_strm.avail_in = (uInt)sizeof(input);
    Bytef outbuf[512];
    memset(outbuf, 0, sizeof(outbuf));
    def_strm.next_out = outbuf;
    def_strm.avail_out = (uInt)sizeof(outbuf);
    int def_reset_ret = deflateReset(&def_strm);

    // step 3: Operate
    int deflate_ret = deflate(&def_strm, 4);
    unsigned int pending = 0;
    int bits = 0;
    int pending_ret = deflatePending(&def_strm, &pending, &bits);
    unsigned long codes_used = inflateCodesUsed(&in_strm);
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(filename, "wb");
    int gzputs_ret = gzputs(gzw, "Hello zlib\n");
    int gzclose_w_ret = gzclose(gzw);

    // step 4: Validate / Cleanup
    gzFile gzr = gzopen(filename, "rb");
    char readbuf[128];
    memset(readbuf, 0, sizeof(readbuf));
    char *gets_ret = gzgets(gzr, readbuf, (int)sizeof(readbuf));
    int gzclose_r_ret = gzclose(gzr);
    int deflate_end_ret = deflateEnd(&def_strm);
    int inflate_end_ret = inflateEnd(&in_strm);

    (void)ver;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)def_reset_ret;
    (void)deflate_ret;
    (void)pending_ret;
    (void)pending;
    (void)bits;
    (void)codes_used;
    (void)gzputs_ret;
    (void)gzclose_w_ret;
    (void)gets_ret;
    (void)gzclose_r_ret;
    (void)deflate_end_ret;
    (void)inflate_end_ret;
    // API sequence test completed successfully
    return 66;
}