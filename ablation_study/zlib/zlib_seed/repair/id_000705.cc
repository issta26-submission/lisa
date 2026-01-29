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
//<ID> 705
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[300];
    memset(plain, 'Z', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain;
    def.avail_in = (uInt)plain_len;
    uLong out_bound = deflateBound(&def, plain_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (size_t)out_bound);
    def.next_out = out_buf;
    def.avail_out = (uInt)out_bound;
    int def_ret = deflate(&def, 4);
    uLong def_total_out = def.total_out;

    // step 2: Configure
    z_stream def_copy;
    memset(&def_copy, 0, sizeof(def_copy));
    int copy_ret = deflateCopy(&def_copy, &def);
    uLong a1 = adler32(0UL, plain, (uInt)(plain_len / 2));
    uLong a2 = adler32(0UL, plain + (plain_len / 2), (uInt)(plain_len / 2));
    off64_t part_len = (off64_t)(plain_len / 2);
    uLong combined = adler32_combine64(a1, a2, part_len);

    // step 3: Operate
    gzFile gzw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw = gzwrite(gzw, out_buf, (unsigned int)def_total_out);
    int gp = gzputs(gzw, "END");
    int gclosew = gzclose(gzw);
    gzFile gzr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int ch1 = gzgetc(gzr);
    int unret = gzungetc(ch1, gzr);
    int ch2 = gzgetc(gzr);
    int eof_flag = gzeof(gzr);
    int gcloser = gzclose(gzr);

    // step 4: Cleanup / Validate
    int def_end_ret = deflateEnd(&def);
    int def_copy_end_ret = deflateEnd(&def_copy);
    free(out_buf);
    (void)version;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_total_out;
    (void)copy_ret;
    (void)a1;
    (void)a2;
    (void)part_len;
    (void)combined;
    (void)gw;
    (void)gp;
    (void)gclosew;
    (void)ch1;
    (void)unret;
    (void)ch2;
    (void)eof_flag;
    (void)gcloser;
    (void)def_end_ret;
    (void)def_copy_end_ret;
    // API sequence test completed successfully
    return 66;
}