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
//<ID> 701
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain_in[256];
    memset(plain_in, 'Z', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    uLong out_bound = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);

    z_stream def;
    z_stream def_copy;
    memset(&def, 0, sizeof(def));
    memset(&def_copy, 0, sizeof(def_copy));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain_in;
    def.avail_in = (uInt)plain_len;
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;
    int copy_ret = deflateCopy(&def_copy, &def);

    // step 2: Configure
    int def_ret = deflate(&def_copy, 4);
    uLong compressed_size = def_copy.total_out;
    int def_copy_end = deflateEnd(&def_copy);
    int def_end = deflateEnd(&def);

    uLong a1 = adler32(0UL, plain_in, (uInt)(plain_len / 2));
    uLong a2 = adler32(0UL, plain_in + (plain_len / 2), (uInt)(plain_len - (plain_len / 2)));
    off64_t half_len = (off64_t)(plain_len / 2);
    uLong combined = adler32_combine64(a1, a2, half_len);

    // step 3: Operate
    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_w = gzwrite(gz, plain_in, (unsigned int)plain_len);
    int gz_close = gzclose(gz);
    gzFile gz2 = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int c1 = gzgetc(gz2);
    int ungot = gzungetc(c1, gz2);
    int c2 = gzgetc(gz2);
    int eof_flag = gzeof(gz2);
    int gz2_close = gzclose(gz2);

    // step 4: Validate / Cleanup
    free(comp_buf);
    (void)version;
    (void)out_bound;
    (void)def_init_ret;
    (void)copy_ret;
    (void)def_ret;
    (void)compressed_size;
    (void)def_copy_end;
    (void)def_end;
    (void)a1;
    (void)a2;
    (void)half_len;
    (void)combined;
    (void)gz_w;
    (void)gz_close;
    (void)c1;
    (void)ungot;
    (void)c2;
    (void)eof_flag;
    (void)gz2_close;
    // API sequence test completed successfully
    return 66;
}