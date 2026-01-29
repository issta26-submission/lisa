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
//<ID> 8
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    z_stream deflate_strm;
    z_stream inflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    deflateInit_(&deflate_strm, 6, zlibVersion(), sizeof(z_stream));
    inflateInit_(&inflate_strm, zlibVersion(), sizeof(z_stream));

    // step 2: Configure
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb+");
    gzbuffer(gf, 4096);

    // step 3: Operate / Validate
    char input[] = "Hello zlib";
    uInt in_len = (uInt)(sizeof(input) - 1);
    deflate_strm.next_in = (Bytef *)input;
    deflate_strm.avail_in = in_len;
    uLong outBound = deflateBound(&deflate_strm, in_len);
    Bytef * outBuf = new Bytef[outBound + 1];
    deflate_strm.next_out = outBuf;
    deflate_strm.avail_out = (uInt)outBound;
    deflate(&deflate_strm, 4); // Z_FINISH (typical value)
    uLong compressed_size = outBound - deflate_strm.avail_out;
    gzwrite(gf, outBuf, (unsigned int)compressed_size);
    off_t current_offset = gzoffset(gf);
    unsigned long codes_used = inflateCodesUsed(&inflate_strm);
    (void)current_offset;
    (void)codes_used;

    // step 4: Cleanup
    deflateEnd(&deflate_strm);
    inflateEnd(&inflate_strm);
    gzclose(gf);
    delete [] outBuf;

    // API sequence test completed successfully
    return 66;
}