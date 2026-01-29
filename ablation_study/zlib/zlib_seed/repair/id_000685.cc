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
//<ID> 685
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[256];
    memset(plain, 'A', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);
    uLong comp_bound = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress(comp_buf, &comp_len, plain, plain_len);

    // step 2: Configure
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int inf_init_ret = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)comp_len;
    Bytef decomp_buf[256];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    strm.next_out = decomp_buf;
    strm.avail_out = (uInt)plain_len;

    // step 3: Operate
    int inf_ret1 = inflate(&strm, 4);
    int inf_reset_ret = inflateReset(&strm);
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)comp_len;
    strm.next_out = decomp_buf;
    strm.avail_out = (uInt)plain_len;
    int inf_ret2 = inflate(&strm, 4);
    int inf_end_ret = inflateEnd(&strm);

    gzFile gz = gzopen64("test_zlib_api_sequence_tmp.gz", "wb");
    int gzwrite_ret = gzwrite(gz, comp_buf, (unsigned int)comp_len);
    off64_t gz_off = gzoffset64(gz);
    int gzclose_ret = gzclose(gz);

    uLong ad1 = adler32(0L, plain, (uInt)plain_len);
    uLong combined = adler32_combine64(ad1, ad1, (off64_t)plain_len);

    // step 4: Validate / Cleanup
    free(comp_buf);
    (void)version;
    (void)comp_bound;
    (void)comp_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)inf_ret1;
    (void)inf_reset_ret;
    (void)inf_ret2;
    (void)inf_end_ret;
    (void)gzwrite_ret;
    (void)gz_off;
    (void)gzclose_ret;
    (void)ad1;
    (void)combined;
    (void)decomp_buf;
    (void)plain_len;
    // API sequence test completed successfully
    return 66;
}