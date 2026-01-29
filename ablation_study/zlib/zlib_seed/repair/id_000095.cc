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
//<ID> 95
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "This is a sample input to compress and then inflate using zlib APIs.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[4096];
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    Bytef decomp_buf[4096];
    gzFile gzf;
    const char * ver;
    int ret_compress;
    int ret_gzwrite;
    off_t seek_pos;
    int ret_inflate_init;
    int ret_inflate;
    int ret_inflate_end;
    int ret_gzclose;

    // step 2: Setup (initialize stream and prepare compressed buffer)
    memset(&strm, 0, sizeof(strm));
    ver = zlibVersion();
    ret_compress = compress(comp_buf, &comp_len, input_buf, input_len);

    // step 3: Core operations (write compressed data to a .gz file, reposition, initialize inflate, perform inflate)
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    seek_pos = gzseek(gzf, 0, 0);
    ret_inflate_init = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)comp_len;
    strm.next_out = decomp_buf;
    strm.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate = inflate(&strm, 0);

    // step 4: Validate & Cleanup (end inflate and close gz file)
    ret_inflate_end = inflateEnd(&strm);
    ret_gzclose = gzclose(gzf);

    (void)ret_compress;
    (void)ret_gzwrite;
    (void)seek_pos;
    (void)ret_inflate_init;
    (void)ret_inflate;
    (void)ret_inflate_end;
    (void)ret_gzclose;
    (void)ver;
    (void)input_len;
    (void)comp_len;
    (void)strm.total_out;

    // API sequence test completed successfully
    return 66;
}