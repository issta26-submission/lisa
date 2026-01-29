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
//<ID> 90
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "Example payload for zlib API sequencing and validation.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[2048];
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    Bytef decomp_buf[2048];
    gzFile gzf;
    const char * ver;
    int ret_compress;
    int ret_inflate_init;
    int ret_inflate;
    int ret_inflate_end;
    int ret_gzwrite1;
    int ret_gzwrite2;
    off_t seek_ret;

    // step 2: Setup (initialize inflate stream and obtain zlib version)
    memset(&strm, 0, sizeof(strm));
    ver = zlibVersion();
    ret_inflate_init = inflateInit_(&strm, ver, (int)sizeof(z_stream));

    // step 3: Core operations (compress data and write to gz file, seek and write again)
    ret_compress = compress(comp_buf, &comp_len, input_buf, input_len);
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite1 = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    seek_ret = gzseek(gzf, (off_t)0, 0);
    ret_gzwrite2 = gzwrite(gzf, comp_buf, (unsigned int)comp_len);

    // step 4: Validate & Cleanup (inflate compressed buffer to validate, end stream, close file)
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)comp_len;
    strm.next_out = decomp_buf;
    strm.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate = inflate(&strm, 0);
    ret_inflate_end = inflateEnd(&strm);
    gzclose(gzf);

    (void)ver;
    (void)ret_compress;
    (void)ret_inflate_init;
    (void)ret_inflate;
    (void)ret_inflate_end;
    (void)ret_gzwrite1;
    (void)ret_gzwrite2;
    (void)seek_ret;
    (void)input_len;
    (void)comp_len;
    (void)strm.total_out;

    // API sequence test completed successfully
    return 66;
}