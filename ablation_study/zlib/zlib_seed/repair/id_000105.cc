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
//<ID> 105
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "Sample input data for zlib API sequencing: compress, write, read, inflate, and CRC check.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[4096];
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    Bytef decomp_buf[4096];
    const char * ver;
    gzFile gzf_write;
    gzFile gzf_read;
    uLong crc;
    off64_t offset;
    int ret_compress;
    int ret_gzwrite;
    int ret_gzread;
    int ret_gzeof;
    int ret_inflate_init;
    int ret_inflate;
    int ret_inflate_end;
    int ret_inflate_back_end;
    int ret_gzclose_r;

    // Step 2: Setup (initialize stream and get version)
    memset(&strm, 0, sizeof(strm));
    ver = zlibVersion();

    // Step 3: Operate (compress, write to gz, read back, compute CRC, inflate)
    ret_compress = compress(comp_buf, &comp_len, input_buf, input_len);
    gzf_write = gzopen64("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gzf_write, comp_buf, (unsigned int)comp_len);
    gzclose(gzf_write);
    gzf_read = gzopen64("test_zlib_api_sequence.tmp.gz", "rb");
    offset = gzoffset64(gzf_read);
    crc = crc32(0L, input_buf, (uInt)input_len);
    ret_gzeof = gzeof(gzf_read);
    ret_gzread = gzread(gzf_read, comp_buf, (unsigned int)comp_len);
    ret_inflate_init = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)comp_len;
    strm.next_out = decomp_buf;
    strm.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate = inflate(&strm, 0);

    // Step 4: Validate & Cleanup (end inflate, backend cleanup, close read gz)
    ret_inflate_end = inflateEnd(&strm);
    ret_inflate_back_end = inflateBackEnd(&strm);
    ret_gzclose_r = gzclose_r(gzf_read);

    (void)ret_compress;
    (void)ret_gzwrite;
    (void)ret_gzread;
    (void)ret_gzeof;
    (void)ret_inflate_init;
    (void)ret_inflate;
    (void)ret_inflate_end;
    (void)ret_inflate_back_end;
    (void)ret_gzclose_r;
    (void)ver;
    (void)offset;
    (void)crc;
    (void)strm.total_out;
    (void)comp_len;
    (void)input_len;

    // API sequence test completed successfully
    return 66;
}