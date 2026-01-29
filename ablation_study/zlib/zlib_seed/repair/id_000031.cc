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
//<ID> 31
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
    unsigned char input_data[] = "Example data for zlib compression and testing.";
    unsigned int input_len = (unsigned int)(sizeof(input_data) - 1);
    unsigned char outbuf[512];
    gzFile gzf;
    off_t file_offset;
    unsigned long codes_used;
    int ret_deflate_init;
    int ret_inflate_init;
    int ret_deflate;
    int ret_deflate_end;
    int ret_inflate_end;
    int bytes_written;
    int ret_gzclose;

    // step 2: Setup (initialize streams and open gzip file)
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    ret_deflate_init = deflateInit_(&deflate_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    ret_inflate_init = inflateInit_(&inflate_strm, zlibVersion(), (int)sizeof(z_stream));
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");

    // step 3: Operate & Validate (compress input, write to gzip, inspect offset and inflate codes)
    deflate_strm.next_in = (Bytef *)input_data;
    deflate_strm.avail_in = (uInt)input_len;
    deflate_strm.next_out = (Bytef *)outbuf;
    deflate_strm.avail_out = (uInt)sizeof(outbuf);
    deflate_strm.total_out = 0;
    ret_deflate = deflate(&deflate_strm, 0);
    unsigned int compressed_len = (unsigned int)deflate_strm.total_out;
    bytes_written = gzwrite(gzf, outbuf, compressed_len);
    file_offset = gzoffset(gzf);
    codes_used = inflateCodesUsed(&inflate_strm);
    gzclearerr(gzf);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&deflate_strm);
    ret_inflate_end = inflateEnd(&inflate_strm);
    ret_gzclose = gzclose(gzf);

    (void)ret_deflate_init; (void)ret_inflate_init; (void)ret_deflate; (void)compressed_len;
    (void)bytes_written; (void)file_offset; (void)codes_used; (void)ret_deflate_end; (void)ret_inflate_end; (void)ret_gzclose;
    // API sequence test completed successfully
    return 66;
}