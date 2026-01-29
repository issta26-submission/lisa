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
//<ID> 101
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream inflate_strm;
    Bytef input_buf[] = "zlib API sequence test input data for compression, CRC and gz file operations.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[2048];
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    Bytef decomp_buf[2048];
    const char * ver;
    uLong crc_init;
    int ret_compress;
    gzFile gzf;
    int ret_gzwrite;
    off64_t offset64;
    int eof_flag;
    int ret_inflate_init;
    int ret_inflate;
    int ret_inflate_back_end;
    int ret_inflate_end;
    int ret_gzclose_r;

    // step 2: Setup
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    ver = zlibVersion();
    crc_init = crc32(0L, input_buf, (uInt)input_len);

    // step 3: Core operations (compress -> write gz -> query offset & eof -> inflate)
    ret_compress = compress2(comp_buf, &comp_len, input_buf, input_len, 6);
    gzf = gzopen64("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    offset64 = gzoffset64(gzf);
    eof_flag = gzeof(gzf);

    ret_inflate_init = inflateInit_(&inflate_strm, ver, (int)sizeof(z_stream));
    inflate_strm.next_in = comp_buf;
    inflate_strm.avail_in = (uInt)comp_len;
    inflate_strm.next_out = decomp_buf;
    inflate_strm.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate = inflate(&inflate_strm, 0);

    // step 4: Cleanup and validation
    ret_inflate_back_end = inflateBackEnd(&inflate_strm);
    ret_inflate_end = inflateEnd(&inflate_strm);
    ret_gzclose_r = gzclose_r(gzf);

    (void)ver;
    (void)crc_init;
    (void)ret_compress;
    (void)ret_gzwrite;
    (void)offset64;
    (void)eof_flag;
    (void)ret_inflate_init;
    (void)ret_inflate;
    (void)ret_inflate_back_end;
    (void)ret_inflate_end;
    (void)ret_gzclose_r;
    (void)inflate_strm.total_out;
    (void)comp_len;
    (void)input_len;

    // API sequence test completed successfully
    return 66;
}