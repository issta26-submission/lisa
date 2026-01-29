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
//<ID> 756
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[512];
    memset(plain, 'Z', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);

    z_stream s1;
    memset(&s1, 0, sizeof(s1));
    int init_ret = deflateInit_(&s1, 6, version, (int)sizeof(z_stream));
    s1.next_in = plain;
    s1.avail_in = (uInt)plain_len;

    // step 2: Configure
    uLong bound = deflateBound(&s1, plain_len);
    Bytef *stream_out = (Bytef *)malloc((size_t)bound);
    memset(stream_out, 0, (size_t)bound);
    s1.next_out = stream_out;
    s1.avail_out = (uInt)bound;

    z_stream s2;
    memset(&s2, 0, sizeof(s2));
    int copy_ret = deflateCopy(&s2, &s1);
    int reset_ret = deflateReset(&s2);

    // step 3: Operate
    int d1 = deflate(&s1, 4);
    uLong s1_out = s1.total_out;
    // prepare s2 to produce additional output safely
    s2.next_in = plain;
    s2.avail_in = (uInt)plain_len;
    s2.next_out = stream_out + (size_t)s1_out;
    s2.avail_out = (uInt)(bound - s1_out);
    int d2 = deflate(&s2, 4);
    uLong s2_out = s2.total_out;

    uLong total_stream_out = s1_out + s2_out;
    uLong comp_bound = compressBound(plain_len);
    uLongf comp_len = (uLongf)comp_bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    int comp_ret = compress(comp_buf, &comp_len, plain, plain_len);

    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gw = gzopen(fname, "wb");
    int gwr = gzwrite(gw, stream_out, (unsigned int)total_stream_out);
    int gclose_w = gzclose(gw);

    gzFile gr = gzopen(fname, "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(total_stream_out + 16));
    memset(read_buf, 0, (size_t)(total_stream_out + 16));
    z_size_t read_count = gzfread(read_buf, 1, (z_size_t)total_stream_out, gr);
    int gclose_r = gzclose(gr);

    // step 4: Validate / Cleanup
    int end1 = deflateEnd(&s1);
    int end2 = deflateEnd(&s2);
    free(stream_out);
    free(comp_buf);
    free(read_buf);

    (void)version;
    (void)init_ret;
    (void)copy_ret;
    (void)reset_ret;
    (void)d1;
    (void)d2;
    (void)s1_out;
    (void)s2_out;
    (void)total_stream_out;
    (void)comp_bound;
    (void)comp_len;
    (void)comp_ret;
    (void)gwr;
    (void)gclose_w;
    (void)read_count;
    (void)gclose_r;
    (void)end1;
    (void)end2;
    // API sequence test completed successfully
    return 66;
}