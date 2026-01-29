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
//<ID> 32
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm_def;
    z_stream strm_inf;
    Bytef inbuf[] = "Example data for zlib compression and decompression roundtrip.";
    uInt in_len = (uInt)(sizeof(inbuf) - 1);
    Bytef compbuf[512];
    Bytef decompbuf[512];
    gzFile gzf;
    int ret_deflate_init;
    int ret_inflate_init;
    int ret_deflate;
    int ret_inflate;
    int ret_deflate_end;
    int ret_inflate_end;
    unsigned int write_len;
    int bytes_written;
    off_t offset;
    unsigned long codes_used;

    // step 2: Setup (initialize streams)
    memset(&strm_def, 0, sizeof(strm_def));
    memset(&strm_inf, 0, sizeof(strm_inf));
    ret_deflate_init = deflateInit_(&strm_def, 6, zlibVersion(), (int)sizeof(z_stream));
    ret_inflate_init = inflateInit_(&strm_inf, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Core operations (compress, write, read offset, clear errors, decompress, query codes used)
    strm_def.next_in = (Bytef *)inbuf;
    strm_def.avail_in = in_len;
    strm_def.next_out = compbuf;
    strm_def.avail_out = (uInt)sizeof(compbuf);
    ret_deflate = deflate(&strm_def, 0);
    write_len = (unsigned int)strm_def.total_out;
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    bytes_written = gzwrite(gzf, compbuf, write_len);
    offset = gzoffset(gzf);
    gzclearerr(gzf);

    strm_inf.next_in = compbuf;
    strm_inf.avail_in = (uInt)write_len;
    strm_inf.next_out = decompbuf;
    strm_inf.avail_out = (uInt)sizeof(decompbuf);
    ret_inflate = inflate(&strm_inf, 0);
    codes_used = inflateCodesUsed(&strm_inf);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&strm_def);
    ret_inflate_end = inflateEnd(&strm_inf);
    gzclose(gzf);

    (void)ret_deflate_init; (void)ret_inflate_init; (void)ret_deflate; (void)ret_inflate;
    (void)write_len; (void)bytes_written; (void)offset; (void)codes_used;
    (void)ret_deflate_end; (void)ret_inflate_end;

    // API sequence test completed successfully
    return 66;
}