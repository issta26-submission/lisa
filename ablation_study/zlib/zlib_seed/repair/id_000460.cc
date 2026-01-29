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
//<ID> 460
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int deflate_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    Bytef input[512];
    memset(input, 'A', sizeof(input));
    Bytef outbuf[2048];
    memset(outbuf, 0, sizeof(outbuf));

    // step 2: Configure
    strm.next_in = input;
    strm.avail_in = (uInt)sizeof(input);
    strm.next_out = outbuf;
    strm.avail_out = (uInt)sizeof(outbuf);
    uLong bound = deflateBound(&strm, (uLong)strm.avail_in);
    (void)bound;
    int deflate_reset_ret = deflateReset(&strm);

    // step 3: Operate
    int deflate_ret = deflate(&strm, 1); // flush value chosen as 1
    unsigned int pending_bits_pending = 0;
    int pending_bits_bits = 0;
    int deflate_pending_ret = deflatePending(&strm, &pending_bits_pending, &pending_bits_bits);
    uLong produced_ul = strm.total_out;
    unsigned int produced = (unsigned int)produced_ul;
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gw = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gw, outbuf, produced);
    int gzclose_w_ret = gzclose(gw);
    gzFile gr = gzopen(filename, "rb");
    char readline[256];
    memset(readline, 0, sizeof(readline));
    char *gzgets_ret = gzgets(gr, readline, (int)sizeof(readline));
    int gzclose_r_ret = gzclose(gr);

    // step 4: Validate / Cleanup
    unsigned long codes_used = inflateCodesUsed(&strm);
    int deflate_end_ret = deflateEnd(&strm);

    (void)ver;
    (void)deflate_init_ret;
    (void)deflate_reset_ret;
    (void)deflate_ret;
    (void)deflate_pending_ret;
    (void)pending_bits_pending;
    (void)pending_bits_bits;
    (void)produced_ul;
    (void)filename;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)gzgets_ret;
    (void)gzclose_r_ret;
    (void)readline;
    (void)codes_used;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}