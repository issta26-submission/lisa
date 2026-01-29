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
//<ID> 39
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
    Bytef inbuf[] = "Example data to compress and then inflate via zlib APIs.";
    uInt in_len = (uInt)(sizeof(inbuf) - 1);
    Bytef comp_buf[512];
    Bytef decomp_buf[512];
    gzFile gzf;
    int ret_deflate_init;
    int ret_inflate_init;
    int ret_deflate;
    int ret_inflate;
    int ret_deflate_end;
    int ret_inflate_end;
    int bytes_written;
    unsigned int compressed_len;
    off_t offset;
    unsigned long codes_used;

    // step 2: Setup (initialize streams)
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    ret_deflate_init = deflateInit_(&deflate_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    ret_inflate_init = inflateInit_(&inflate_strm, zlibVersion(), (int)sizeof(z_stream));
    deflate_strm.next_in = (Bytef *)inbuf;
    deflate_strm.avail_in = in_len;
    deflate_strm.next_out = comp_buf;
    deflate_strm.avail_out = (uInt)sizeof(comp_buf);

    // step 3: Core operations (compress, write to gz, read offset, clear errors, inflate, inspect codes)
    ret_deflate = deflate(&deflate_strm, 0);
    compressed_len = (unsigned int)deflate_strm.total_out;
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    bytes_written = gzwrite(gzf, comp_buf, compressed_len);
    offset = gzoffset(gzf);
    gzclearerr(gzf);
    inflate_strm.next_in = comp_buf;
    inflate_strm.avail_in = (uInt)compressed_len;
    inflate_strm.next_out = decomp_buf;
    inflate_strm.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate = inflate(&inflate_strm, 0);
    codes_used = inflateCodesUsed(&inflate_strm);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&deflate_strm);
    ret_inflate_end = inflateEnd(&inflate_strm);
    gzclose(gzf);

    (void)ret_deflate_init;
    (void)ret_inflate_init;
    (void)ret_deflate;
    (void)ret_inflate;
    (void)bytes_written;
    (void)offset;
    (void)codes_used;
    (void)ret_deflate_end;
    (void)ret_inflate_end;

    // API sequence test completed successfully
    return 66;
}