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
//<ID> 202
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
    Bytef input_buf[] = "zlib API sequence test payload for deflateInit_, compress2, gzopen64, gzwrite, gzrewind and inflateBackEnd.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[65536];
    uLong comp_len = (uLong)sizeof(comp_buf);
    unsigned char window[32768];
    const char *ver;
    gzFile gz;
    int ret_deflate_init;
    int ret_deflate_end;
    int ret_compress2;
    int ret_inflate_back_init;
    int ret_inflate_back_end;
    int ret_gz_write1;
    int ret_gz_rewind;
    int ret_gz_write2;
    int ret_gz_close;

    // step 2: Setup
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(comp_buf, 0, sizeof(comp_buf));
    memset(window, 0, sizeof(window));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_inflate_back_init = inflateBackInit_(&inf_strm, 15, window, ver, (int)sizeof(z_stream));
    ret_compress2 = compress2(comp_buf, &comp_len, input_buf, input_len, 6);

    // step 3: Core operations
    gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    ret_gz_write1 = gzwrite(gz, (voidpc)comp_buf, (unsigned int)comp_len);
    ret_gz_rewind = gzrewind(gz);
    ret_gz_write2 = gzwrite(gz, (voidpc)comp_buf, (unsigned int)(comp_len > 1 ? comp_len / 2u : comp_len));

    // step 4: Cleanup and validation
    ret_deflate_end = deflateEnd(&def_strm);
    ret_inflate_back_end = inflateBackEnd(&inf_strm);
    ret_gz_close = gzclose(gz);
    (void)ret_deflate_init;
    (void)ret_deflate_end;
    (void)ret_compress2;
    (void)ret_inflate_back_init;
    (void)ret_inflate_back_end;
    (void)ret_gz_write1;
    (void)ret_gz_rewind;
    (void)ret_gz_write2;
    (void)ret_gz_close;
    (void)ver;
    (void)input_len;
    (void)comp_len;
    // API sequence test completed successfully
    return 66;
}