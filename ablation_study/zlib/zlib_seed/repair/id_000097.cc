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
//<ID> 97
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    z_stream strm;
    Bytef input_buf[] = "Zlib API sequencing: compress, write to gz, seek, inflate.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[2048];
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    Bytef decomp_buf[2048];
    const char * ver;
    gzFile gzf;
    off_t seek_ret;
    int ret_compress;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    int ret_gzwrite;

    // step 2: Setup (zero stream, get version, compress input)
    memset(&strm, 0, sizeof(strm));
    ver = zlibVersion();
    ret_compress = compress(comp_buf, &comp_len, input_buf, input_len);

    // step 3: Core operations (open gz, write compressed data, seek, initialize inflate and run)
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    seek_ret = gzseek(gzf, (off_t)0, SEEK_SET);
    ret_inflate_init = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)comp_len;
    strm.next_out = decomp_buf;
    strm.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate_call = inflate(&strm, 0);

    // step 4: Cleanup (end inflate and close gz)
    ret_inflate_end = inflateEnd(&strm);
    gzclose(gzf);

    (void)ret_compress;
    (void)ret_gzwrite;
    (void)seek_ret;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)ver;
    (void)input_len;
    (void)comp_len;
    (void)strm.total_out;

    // API sequence test completed successfully
    return 66;
}