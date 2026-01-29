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
//<ID> 36
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    unsigned char input_data[] = "Example input data for zlib compression.";
    uInt input_len = (uInt)(sizeof(input_data) - 1);
    unsigned char outbuf[512];
    uInt outbuf_len = (uInt)sizeof(outbuf);
    gzFile gzf;
    int ret_deflate_init;
    int ret_deflate;
    int ret_deflate_end;
    int bytes_written;
    off_t file_offset;
    unsigned long codes_used;

    // step 2: Setup (initialize deflate stream and open gzip file)
    memset(&strm, 0, sizeof(strm));
    ret_deflate_init = deflateInit_(&strm, 6, zlibVersion(), (int)sizeof(z_stream));
    strm.next_in = (Bytef *)input_data;
    strm.avail_in = input_len;
    strm.next_out = (Bytef *)outbuf;
    strm.avail_out = outbuf_len;
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");

    // step 3: Operate & Validate (perform deflate, write compressed bytes, query file offset and stream usage)
    ret_deflate = deflate(&strm, 0);
    bytes_written = gzwrite(gzf, outbuf, (unsigned int)strm.total_out);
    file_offset = gzoffset(gzf);
    gzclearerr(gzf);
    codes_used = inflateCodesUsed(&strm);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&strm);
    gzclose(gzf);

    // API sequence test completed successfully
    (void)ret_deflate_init; (void)ret_deflate; (void)bytes_written; (void)file_offset; (void)codes_used; (void)ret_deflate_end;
    return 66;
}