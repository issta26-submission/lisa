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
//<ID> 38
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    unsigned char inbuf[] = "Example input data for zlib compression.";
    unsigned int in_len = (unsigned int)(sizeof(inbuf) - 1);
    unsigned char outbuf[512];
    gzFile gzf;
    uLong compressed_len;
    int ret_deflate_init;
    int ret_deflate;
    int bytes_written;
    off_t offset;
    unsigned long codes_used;
    int ret_deflate_end;
    int ret_gzclose;

    // step 2: Setup (initialize deflate stream and open gzip file)
    memset(&strm, 0, sizeof(strm));
    ret_deflate_init = deflateInit_(&strm, 6, zlibVersion(), (int)sizeof(z_stream));
    strm.next_in = (Bytef *)inbuf;
    strm.avail_in = (uInt)in_len;
    strm.next_out = (Bytef *)outbuf;
    strm.avail_out = (uInt)sizeof(outbuf);
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");

    // step 3: Core operations (compress, write, validate)
    ret_deflate = deflate(&strm, 0);
    compressed_len = (uLong)strm.total_out;
    bytes_written = gzwrite(gzf, outbuf, (unsigned int)compressed_len);
    offset = gzoffset(gzf);
    codes_used = inflateCodesUsed(&strm);
    gzclearerr(gzf);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&strm);
    ret_gzclose = gzclose(gzf);

    // API sequence test completed successfully
    (void)ret_deflate_init; (void)ret_deflate; (void)bytes_written; (void)offset; (void)codes_used; (void)ret_deflate_end; (void)ret_gzclose;
    return 66;
}