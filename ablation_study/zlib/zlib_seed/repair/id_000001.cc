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
//<ID> 1
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream deflate_strm;
    z_stream inflate_strm;
    unsigned char inbuf[128];
    unsigned char outbuf[256];
    gzFile gz = 0;
    off_t gz_pos = 0;
    unsigned long codes_used = 0;
    unsigned long combined_metric = 0;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    memset(inbuf, 0, sizeof(inbuf));
    memset(outbuf, 0, sizeof(outbuf));

    // step 2: Setup (streams and gzip)
    deflate_strm.next_in = inbuf;
    deflate_strm.avail_in = (uInt)sizeof(inbuf);
    deflate_strm.next_out = outbuf;
    deflate_strm.avail_out = (uInt)sizeof(outbuf);
    inflate_strm.next_in = outbuf;
    inflate_strm.avail_in = (uInt)sizeof(outbuf);
    inflate_strm.next_out = inbuf;
    inflate_strm.avail_out = (uInt)sizeof(inbuf);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzbuffer(gz, 8192);

    // step 3: Core operations and validation
    deflateInit_(&deflate_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&inflate_strm, zlibVersion(), (int)sizeof(z_stream));
    deflate(&deflate_strm, 0);
    gzwrite(gz, (const void *)"zlib-data", 9u);
    gzflush(gz, 0);
    gz_pos = gzoffset(gz);
    codes_used = inflateCodesUsed(&inflate_strm);
    combined_metric = (unsigned long)gz_pos + codes_used;

    // step 4: Cleanup
    deflateEnd(&deflate_strm);
    inflateEnd(&inflate_strm);
    gzclose(gz);

    // API sequence test completed successfully
    (void)combined_metric;
    return 66;
}