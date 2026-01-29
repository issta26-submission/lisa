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
//<ID> 754
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef *plain = (Bytef *)malloc(256);
    memset(plain, 'A', 256);
    uLong plain_len = (uLong)256;
    uLongf comp_buf_len = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_buf_len);
    memset(comp_buf, 0, (size_t)comp_buf_len);
    int comp_ret = compress(comp_buf, &comp_buf_len, plain, plain_len);

    // step 2: Configure (write compressed payload into a .gz container)
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzw = gzopen(fname, "wb");
    unsigned int gz_written = gzwrite(gzw, comp_buf, (unsigned int)comp_buf_len);
    int gz_close_w = gzclose(gzw);

    // step 3: Operate (read back via gzfread and exercise deflate stream APIs)
    gzFile gzr = gzopen(fname, "rb");
    Bytef *read_buf = (Bytef *)malloc(256);
    memset(read_buf, 0, 256);
    z_size_t items_read = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)256, gzr);
    int gz_close_r = gzclose(gzr);

    z_stream ds1;
    memset(&ds1, 0, sizeof(ds1));
    int init_ret = deflateInit_(&ds1, 6, version, (int)sizeof(z_stream));
    int reset_ret = deflateReset(&ds1);
    z_stream ds2;
    memset(&ds2, 0, sizeof(ds2));
    int copy_ret = deflateCopy(&ds2, &ds1);

    // step 4: Validate / Cleanup
    int end1 = deflateEnd(&ds1);
    int end2 = deflateEnd(&ds2);
    free(plain);
    free(comp_buf);
    free(read_buf);
    (void)version;
    (void)plain_len;
    (void)comp_buf_len;
    (void)comp_ret;
    (void)gz_written;
    (void)gz_close_w;
    (void)items_read;
    (void)gz_close_r;
    (void)init_ret;
    (void)reset_ret;
    (void)copy_ret;
    (void)end1;
    (void)end2;
    // API sequence test completed successfully
    return 66;
}