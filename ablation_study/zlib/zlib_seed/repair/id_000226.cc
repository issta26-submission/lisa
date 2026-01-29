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
//<ID> 226
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream inf_strm;
    Bytef input_buf[] = "zlib API sequence test payload: compress with compress2, compute CRC, write to gzFile, inflate, verify CRC, cleanup.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    uLong comp_len;
    uLong bound;
    Bytef outbuf[65536];
    uLong out_len;
    const char *ver;
    gzFile gz;
    uLong crc_orig;
    uLong crc_decomp;
    int ret_compress;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    int ret_gzwrite;
    int ret_gz_close;

    // step 2: Setup
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    bound = compressBound(input_len);
    comp_len = (bound <= (uLong)sizeof(compbuf)) ? bound : (uLong)sizeof(compbuf);
    ret_compress = compress2(compbuf, &comp_len, input_buf, input_len, 6);
    crc_orig = crc32(0L, input_buf, (uInt)input_len);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gzwrite = gzwrite(gz, (voidpc)compbuf, (unsigned int)comp_len);

    // step 3: Core operations
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    out_len = inf_strm.total_out;
    crc_decomp = crc32(0L, outbuf, (uInt)out_len);

    // step 4: Cleanup and validation
    ret_inflate_end = inflateEnd(&inf_strm);
    ret_gz_close = gzclose(gz);
    (void)ret_compress;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)ret_gzwrite;
    (void)ret_gz_close;
    (void)bound;
    (void)comp_len;
    (void)out_len;
    (void)ver;
    (void)crc_orig;
    (void)crc_decomp;
    // API sequence test completed successfully
    return 66;
}