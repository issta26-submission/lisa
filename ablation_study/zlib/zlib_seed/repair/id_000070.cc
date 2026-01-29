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
//<ID> 70
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream inflate_strm;
    Bytef input_buf[] = "Hello zlib data to compress and then inflate.";
    uInt input_len = (uInt)(sizeof(input_buf) - 1);
    Bytef comp_buf[512];
    uLongf comp_len_f = (uLongf)sizeof(comp_buf);
    Bytef decomp_buf[512];
    uLongf decomp_len_f = (uLongf)sizeof(decomp_buf);
    const char * ver;
    int ret_inflate_init;
    int ret_compress;
    int ret_inflate;
    int bytes_written;
    gzFile gzf;

    // step 2: Setup (initialize stream, get version, compress source, open gz file)
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    ver = zlibVersion();
    ret_inflate_init = inflateInit_(&inflate_strm, ver, (int)sizeof(z_stream));
    ret_compress = compress(comp_buf, &comp_len_f, input_buf, (uLong)input_len);
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");

    // step 3: Core operations (configure inflate stream, perform inflate, write inflated data)
    inflate_strm.next_in = comp_buf;
    inflate_strm.avail_in = (uInt)comp_len_f;
    inflate_strm.next_out = decomp_buf;
    inflate_strm.avail_out = (uInt)decomp_len_f;
    ret_inflate = inflate(&inflate_strm, 0);
    bytes_written = gzwrite(gzf, decomp_buf, (unsigned int)inflate_strm.total_out);

    // step 4: Cleanup (end inflate and close gz file)
    inflateEnd(&inflate_strm);
    gzclose(gzf);

    (void)ver;
    (void)ret_inflate_init;
    (void)ret_compress;
    (void)ret_inflate;
    (void)bytes_written;
    (void)comp_len_f;
    (void)decomp_len_f;

    // API sequence test completed successfully
    return 66;
}