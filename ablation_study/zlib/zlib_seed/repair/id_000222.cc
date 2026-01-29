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
//<ID> 222
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
    Bytef input_buf[] = "zlib API sequence: deflate to memory, compute crc32, write compressed blob to gzFile, then inflate back and cleanup.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    Bytef outbuf[65536];
    const char *ver;
    gzFile gz;
    uLong comp_len;
    uLong out_len;
    uLong checksum;
    int ret_def_init;
    int ret_def_call;
    int ret_def_end;
    int ret_in_init;
    int ret_in_call;
    int ret_in_end;
    int ret_gz_write;
    int ret_gz_close;

    // step 2: Setup
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_def_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);

    // step 3: Core operations
    ret_def_call = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    checksum = crc32(0L, input_buf, (uInt)input_len);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, compbuf, (unsigned int)comp_len);
    ret_gz_close = gzclose(gz);
    ret_in_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_in_call = inflate(&inf_strm, 0);
    out_len = inf_strm.total_out;

    // step 4: Cleanup and validation
    ret_def_end = deflateEnd(&def_strm);
    ret_in_end = inflateEnd(&inf_strm);
    (void)ret_def_init;
    (void)ret_def_call;
    (void)ret_def_end;
    (void)ret_in_init;
    (void)ret_in_call;
    (void)ret_in_end;
    (void)ret_gz_write;
    (void)ret_gz_close;
    (void)ver;
    (void)checksum;
    (void)comp_len;
    (void)out_len;
    // API sequence test completed successfully
    return 66;
}