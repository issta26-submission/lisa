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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    z_stream istrm;
    unsigned char inbuf[] = "Example data for zlib compression and inflateCodesUsed test.";
    unsigned int in_len = (unsigned int)(sizeof(inbuf) - 1);
    unsigned char compbuf[512];
    unsigned char decompbuf[512];
    gzFile gzf;
    off_t offset;
    unsigned long codes_used;
    int ret_deflate_init;
    int ret_deflate;
    int ret_deflate_end;
    int ret_inflate_init;
    int ret_inflate;
    int ret_inflate_end;
    int bytes_written;
    int ret_gzclose;

    // step 2: Setup (initialize streams and open gzip file)
    memset(&strm, 0, sizeof(strm));
    memset(&istrm, 0, sizeof(istrm));
    ret_deflate_init = deflateInit_(&strm, 6, zlibVersion(), (int)sizeof(z_stream));
    strm.next_in = (Bytef *)inbuf;
    strm.avail_in = (uInt)in_len;
    strm.next_out = (Bytef *)compbuf;
    strm.avail_out = (uInt)sizeof(compbuf);
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");

    // step 3: Core operations (compress, write, clearerr/offset, inflate, inspect codes)
    ret_deflate = deflate(&strm, 0);
    unsigned int compressed_len = (unsigned int)strm.total_out;
    bytes_written = gzwrite(gzf, compbuf, compressed_len);
    gzclearerr(gzf);
    offset = gzoffset(gzf);
    ret_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = (Bytef *)compbuf;
    istrm.avail_in = (uInt)compressed_len;
    istrm.next_out = (Bytef *)decompbuf;
    istrm.avail_out = (uInt)sizeof(decompbuf);
    ret_inflate = inflate(&istrm, 0);
    codes_used = inflateCodesUsed(&istrm);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&strm);
    ret_inflate_end = inflateEnd(&istrm);
    ret_gzclose = gzclose(gzf);

    // API sequence test completed successfully
    (void)ret_deflate_init; (void)ret_deflate; (void)bytes_written; (void)offset; (void)ret_inflate_init;
    (void)ret_inflate; (void)codes_used; (void)ret_deflate_end; (void)ret_inflate_end; (void)ret_gzclose;
    return 66;
}