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
//<ID> 141
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream def_strm;
    z_stream inf_strm;
    Bytef input_buf[] = "zlib API sequence test payload to exercise deflateTune, deflate, inflate and gzseek.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[8192];
    uInt comp_buf_size = (uInt)sizeof(comp_buf);
    Bytef out_buf[8192];
    uInt out_buf_size = (uInt)sizeof(out_buf);
    const char * ver;
    int ret_def_init;
    int ret_def_tune;
    int ret_deflate;
    int ret_def_end;
    int ret_inf_init;
    int ret_inflate;
    int ret_inflate_end;
    gzFile gz;
    off_t seek_result;
    int ret_gz_close;
    uLong out_crc;

    // step 2: Setup
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(comp_buf, 0, sizeof(comp_buf));
    memset(out_buf, 0, sizeof(out_buf));
    ver = zlibVersion();
    ret_def_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_inf_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    ret_def_tune = deflateTune(&def_strm, 32, 64, 258, 4096);

    // step 3: Core operations and validation
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = comp_buf_size;
    ret_deflate = deflate(&def_strm, 0);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)def_strm.total_out;
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = out_buf_size;
    ret_inflate = inflate(&inf_strm, 0);
    out_crc = crc32(0UL, out_buf, (uInt)inf_strm.total_out);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, comp_buf, (unsigned int)def_strm.total_out);
    seek_result = gzseek(gz, 0, 0);
    ret_gz_close = gzclose(gz);

    // step 4: Cleanup
    ret_def_end = deflateEnd(&def_strm);
    ret_inflate_end = inflateEnd(&inf_strm);
    (void)ret_def_init;
    (void)ret_def_tune;
    (void)ret_deflate;
    (void)ret_def_end;
    (void)ret_inf_init;
    (void)ret_inflate;
    (void)ret_inflate_end;
    (void)ret_gz_close;
    (void)ver;
    (void)comp_buf_size;
    (void)out_buf_size;
    (void)out_crc;
    (void)seek_result;
    (void)def_strm.total_out;
    (void)inf_strm.total_out;
    // API sequence test completed successfully
    return 66;
}