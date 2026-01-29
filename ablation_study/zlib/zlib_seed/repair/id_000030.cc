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
//<ID> 30
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    const Bytef inbuf[] = "Example data to compress via zlib.";
    uInt in_len = (uInt)(sizeof(inbuf) - 1);
    Bytef outbuf[512];
    gzFile gzf;
    int ret_deflate_init;
    int ret_deflate;
    unsigned int compressed_len;
    int bytes_written;
    off_t file_offset;
    unsigned long codes_used;
    int ret_deflate_end;
    int ret_gzclose;

    // step 2: Setup (initialize stream and open gzip file)
    memset(&strm, 0, sizeof(strm));
    ret_deflate_init = deflateInit_(&strm, 6, zlibVersion(), (int)sizeof(z_stream));
    strm.next_in = (Bytef *)inbuf;
    strm.avail_in = in_len;
    strm.next_out = outbuf;
    strm.avail_out = (uInt)sizeof(outbuf);
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");

    // step 3: Core operations (compress, write, query and validate)
    ret_deflate = deflate(&strm, 0);
    compressed_len = (unsigned int)strm.total_out;
    bytes_written = gzwrite(gzf, outbuf, compressed_len);
    file_offset = gzoffset(gzf);
    gzclearerr(gzf);
    codes_used = inflateCodesUsed(&strm);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&strm);
    ret_gzclose = gzclose(gzf);

    (void)ret_deflate_init; (void)ret_deflate; (void)compressed_len; (void)bytes_written;
    (void)file_offset; (void)codes_used; (void)ret_deflate_end; (void)ret_gzclose;

    // API sequence test completed successfully
    return 66;
}