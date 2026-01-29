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
//<ID> 91
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "Sample input data for zlib API sequence testing.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[1024];
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    Bytef decomp_buf[1024];
    const char *ver;
    gzFile gzf;
    off_t seek_result;
    int ret_compress;
    int ret_gzwrite;
    int ret_inflate_init;
    int ret_inflate_end;

    // step 2: Setup
    memset(&strm, 0, sizeof(strm));
    ver = zlibVersion();
    ret_compress = compress(comp_buf, &comp_len, input_buf, input_len);

    // step 3: Core operations
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    seek_result = gzseek(gzf, 0, 0);
    ret_inflate_init = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)comp_len;
    strm.next_out = decomp_buf;
    strm.avail_out = (uInt)sizeof(decomp_buf);

    // step 4: Cleanup
    ret_inflate_end = inflateEnd(&strm);
    gzclose(gzf);
    (void)ver;
    (void)input_len;
    (void)comp_len;
    (void)ret_compress;
    (void)ret_gzwrite;
    (void)seek_result;
    (void)ret_inflate_init;
    (void)ret_inflate_end;
    // API sequence test completed successfully
    return 66;
}