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
//<ID> 224
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream inf_strm;
    Bytef input_buf[] = "zlib API sequence test payload for compress2 -> inflate -> write gz -> cleanup";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    Bytef outbuf[65536];
    uLong comp_len;
    uLong bound;
    uLong checksum;
    const char *ver;
    gzFile gz;
    int ret_compress;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    int ret_gz_write;
    int ret_gz_close;

    // step 2: Setup
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    checksum = crc32(0L, input_buf, (uInt)input_len);
    bound = compressBound(input_len);
    comp_len = (uLong)sizeof(compbuf);

    // step 3: Core operations
    ret_compress = compress2(compbuf, &comp_len, input_buf, input_len, 6);
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, (voidpc)outbuf, (unsigned int)inf_strm.total_out);
    ret_gz_close = gzclose(gz);

    // step 4: Cleanup and validation
    ret_inflate_end = inflateEnd(&inf_strm);
    (void)ret_compress;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)ret_gz_write;
    (void)ret_gz_close;
    (void)checksum;
    (void)bound;
    (void)comp_len;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}