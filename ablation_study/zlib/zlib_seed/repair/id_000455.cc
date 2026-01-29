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
//<ID> 455
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int deflate_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef input[128];
    memset(input, 'A', sizeof(input));
    uLong input_len = (uLong)sizeof(input);
    Bytef compbuf[256];
    memset(compbuf, 0, sizeof(compbuf));
    uLongf comp_len = (uLongf)sizeof(compbuf);
    int compress_ret = compress(compbuf, &comp_len, input, input_len);

    // step 3: Operate
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz_w = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gz_w, compbuf, (unsigned int)comp_len);
    int gzclose_w_ret = gzclose(gz_w);
    gzFile gz_r = gzopen(filename, "rb");
    Bytef readbuf[256];
    memset(readbuf, 0, sizeof(readbuf));
    int gzread_ret = gzread(gz_r, readbuf, (unsigned int)comp_len);
    int gzclose_r_ret = gzclose(gz_r);

    // step 4: Validate / Cleanup
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t crc0 = crc_tbl[0];
    uLong sample_metric = (uLong)comp_len + (uLong)gzread_ret + (uLong)crc0;
    int deflate_end_ret = deflateEnd(&strm);

    (void)ver;
    (void)deflate_init_ret;
    (void)compress_ret;
    (void)filename;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)gzread_ret;
    (void)gzclose_r_ret;
    (void)sample_metric;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}