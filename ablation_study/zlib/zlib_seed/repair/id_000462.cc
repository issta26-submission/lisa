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
//<ID> 462
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
    input[sizeof(input) - 1] = '\n';
    uInt input_len = (uInt)sizeof(input);

    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));
    int def_reset_ret = deflateReset(&defstrm);

    z_stream infstrm;
    memset(&infstrm, 0, sizeof(infstrm));
    int inf_init_ret = inflateInit_(&infstrm, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef compbuf[65536];
    memset(compbuf, 0, sizeof(compbuf));
    defstrm.next_in = input;
    defstrm.avail_in = input_len;
    defstrm.next_out = compbuf;
    defstrm.avail_out = (uInt)sizeof(compbuf);

    // step 3: Operate
    int deflate_ret = deflate(&defstrm, 4); // 4 == Z_FINISH
    unsigned int pending = 0;
    int pending_bits = 0;
    int deflate_pending_ret = deflatePending(&defstrm, &pending, &pending_bits);
    uLong comp_len = defstrm.total_out;

    infstrm.next_in = compbuf;
    infstrm.avail_in = (uInt)comp_len;
    Bytef outbuf[128];
    memset(outbuf, 0, sizeof(outbuf));
    infstrm.next_out = outbuf;
    infstrm.avail_out = (uInt)sizeof(outbuf);
    int inflate_ret = inflate(&infstrm, 4); // 4 == Z_FINISH
    unsigned long codes_used = inflateCodesUsed(&infstrm);

    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gzw, input, (unsigned int)input_len);
    int gzclose_w_ret = gzclose(gzw);

    gzFile gzr = gzopen(filename, "rb");
    char linebuf[256];
    memset(linebuf, 0, sizeof(linebuf));
    char *gzgets_ret = gzgets(gzr, linebuf, (int)sizeof(linebuf));
    int gzclose_r_ret = gzclose(gzr);

    // step 4: Validate / Cleanup
    int deflate_end_ret = deflateEnd(&defstrm);
    int inflate_end_ret = inflateEnd(&infstrm);

    (void)ver;
    (void)def_init_ret;
    (void)def_reset_ret;
    (void)inf_init_ret;
    (void)deflate_ret;
    (void)pending;
    (void)pending_bits;
    (void)deflate_pending_ret;
    (void)comp_len;
    (void)inflate_ret;
    (void)codes_used;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)gzgets_ret;
    (void)gzclose_r_ret;
    (void)deflate_end_ret;
    (void)inflate_end_ret;
    // API sequence test completed successfully
    return 66;
}