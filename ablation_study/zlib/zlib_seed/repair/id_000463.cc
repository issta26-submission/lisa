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
//<ID> 463
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
    Bytef inbuf[1024];
    memset(inbuf, 'A', sizeof(inbuf));
    strm.next_in = inbuf;
    strm.avail_in = (uInt)sizeof(inbuf);
    Bytef outbuf[65536];
    memset(outbuf, 0, sizeof(outbuf));
    strm.next_out = outbuf;
    strm.avail_out = (uInt)sizeof(outbuf);

    // step 2: Configure / Reset
    int deflate_reset_ret = deflateReset(&strm);

    // step 3: Operate
    int deflate_ret = deflate(&strm, 4);
    unsigned int pending = 0;
    int bits = 0;
    int deflate_pending_ret = deflatePending(&strm, &pending, &bits);
    unsigned long codes_used = inflateCodesUsed(&strm);
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gzw, outbuf, (unsigned int)strm.total_out);
    int gzclose_w_ret = gzclose(gzw);
    gzFile gzr = gzopen(filename, "rb");
    char readbuf[512];
    memset(readbuf, 0, sizeof(readbuf));
    char *gzgets_ret = gzgets(gzr, readbuf, (int)sizeof(readbuf));
    int gzclose_r_ret = gzclose(gzr);

    // step 4: Validate / Cleanup
    int deflate_end_ret = deflateEnd(&strm);

    (void)ver;
    (void)deflate_init_ret;
    (void)deflate_reset_ret;
    (void)deflate_ret;
    (void)pending;
    (void)bits;
    (void)deflate_pending_ret;
    (void)codes_used;
    (void)filename;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)gzgets_ret;
    (void)gzclose_r_ret;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}