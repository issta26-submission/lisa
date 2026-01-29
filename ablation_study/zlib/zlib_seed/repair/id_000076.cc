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
//<ID> 76
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    const char *ver;
    Bytef input[] = "Hello zlib test data for compress->gzwrite->inflate sequence.";
    uLong input_len = (uLong)(sizeof(input) - 1);
    uLong bound = compressBound(input_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    uLongf comp_len = (uLongf)bound;
    Bytef decomp_buf[512];
    gzFile gzf;
    int compress_ret;
    int gzwrite_ret;
    int inflate_init_ret;
    int inflate_ret;
    int inflate_end_ret;
    int gzclose_ret;

    // step 2: Setup (initialize stream, get version, compress source, open gz file)
    memset(&strm, 0, sizeof(strm));
    ver = zlibVersion();
    compress_ret = compress2(comp_buf, &comp_len, input, input_len, 6);
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzwrite_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_len);

    // step 3: Core operations (initialize inflate with version, set buffers, inflate)
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)comp_len;
    strm.next_out = decomp_buf;
    strm.avail_out = (uInt)sizeof(decomp_buf);
    inflate_init_ret = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    inflate_ret = inflate(&strm, 0);

    // step 4: Cleanup (end inflate, close gz, free buffers)
    inflate_end_ret = inflateEnd(&strm);
    gzclose_ret = gzclose(gzf);
    free(comp_buf);

    (void)ver;
    (void)compress_ret;
    (void)gzwrite_ret;
    (void)inflate_init_ret;
    (void)inflate_ret;
    (void)inflate_end_ret;
    (void)gzclose_ret;

    // API sequence test completed successfully
    return 66;
}