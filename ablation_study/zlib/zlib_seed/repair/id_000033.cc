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
//<ID> 33
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream defstrm;
    z_stream infstrm;
    unsigned char inbuf[] = "Example data that will be compressed and written to a gzip file.";
    unsigned int in_len = (unsigned int)(sizeof(inbuf) - 1);
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
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&infstrm, 0, sizeof(infstrm));
    ret_deflate_init = deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    ret_inflate_init = inflateInit_(&infstrm, zlibVersion(), (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)inbuf;
    defstrm.avail_in = (uInt)in_len;
    defstrm.next_out = (Bytef *)outbuf;
    defstrm.avail_out = (uInt)sizeof(outbuf);
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");

    // step 3: Core operations (compress, write, inspect file and inflater state)
    ret_deflate = deflate(&defstrm, 0);
    unsigned int compressed_len = (unsigned int)defstrm.total_out;
    bytes_written = gzwrite(gzf, outbuf, compressed_len);
    file_offset = gzoffset(gzf);
    gzclearerr(gzf);
    codes_used = inflateCodesUsed(&infstrm);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&defstrm);
    ret_inflate_end = inflateEnd(&infstrm);
    ret_gzclose = gzclose(gzf);

    (void)ret_deflate_init; (void)ret_inflate_init; (void)ret_deflate; (void)compressed_len;
    (void)bytes_written; (void)file_offset; (void)codes_used; (void)ret_deflate_end;
    (void)ret_inflate_end; (void)ret_gzclose;
    // API sequence test completed successfully
    return 66;
}