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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream deflateStrm;
    z_stream inflateStrm;
    gzFile gzfile;
    unsigned char in_buf[64];
    const char write_data[] = "Hello, zlib fuzz test";
    unsigned int write_len = (unsigned int)(sizeof(write_data) - 1);
    off_t file_offset;
    unsigned long codes_used;
    int rc_deflate_init;
    int rc_inflate_init;
    int rc_gzbuffer;
    int rc_deflate_end;
    int rc_inflate_end;
    // zero-initialize structures and buffers
    memset(&deflateStrm, 0, sizeof(deflateStrm));
    memset(&inflateStrm, 0, sizeof(inflateStrm));
    memset(in_buf, 0, sizeof(in_buf));

    // step 2: Setup (initialize zlib streams and open gz file)
    rc_deflate_init = deflateInit_(&deflateStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    rc_inflate_init = inflateInit_(&inflateStrm, zlibVersion(), (int)sizeof(z_stream));
    gzfile = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    rc_gzbuffer = gzbuffer(gzfile, 4096);

    // step 3: Operate (write compressed file and query state)
    gzwrite(gzfile, (voidpc)write_data, write_len);
    file_offset = gzoffset(gzfile);
    codes_used = inflateCodesUsed(&inflateStrm);

    // step 4: Validate & Cleanup
    rc_deflate_end = deflateEnd(&deflateStrm);
    rc_inflate_end = inflateEnd(&inflateStrm);
    gzclose(gzfile);

    return 66; // API sequence test completed successfully
}