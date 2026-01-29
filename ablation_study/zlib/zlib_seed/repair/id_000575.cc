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
//<ID> 575
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef src[128];
    memset(src, 'Z', sizeof(src));
    uLong src_len = (uLong)sizeof(src);
    Bytef comp_buf[512];
    memset(comp_buf, 0, sizeof(comp_buf));
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    int comp_ret = compress(comp_buf, &comp_len, src, src_len);

    // step 2: Setup / Configure
    gzFile gf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzFile gf64 = gzopen64("test_zlib_api_sequence64.tmp.gz", "wb");
    int gwritten1 = gzwrite(gf, comp_buf, (unsigned int)comp_len);
    int gwritten2 = gzwrite(gf64, src, (unsigned int)src_len);

    // step 3: Operate
    off_t pos0 = (off_t)0;
    off_t seek_ret1 = gzseek(gf, pos0, 0);
    int gwritten3 = gzwrite(gf, src, (unsigned int)16);
    off_t seek_ret2 = gzseek(gf64, (off_t)0, 0);

    // step 4: Validate / Cleanup
    int gclose1 = gzclose(gf);
    int gclose2 = gzclose(gf64);
    Bytef decomp_buf[512];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    uLongf decomp_len = (uLongf)sizeof(decomp_buf);
    int uncomp_ret = uncompress(decomp_buf, &decomp_len, comp_buf, comp_len);

    (void)ver;
    (void)comp_ret;
    (void)gwritten1;
    (void)gwritten2;
    (void)seek_ret1;
    (void)gwritten3;
    (void)seek_ret2;
    (void)gclose1;
    (void)gclose2;
    (void)uncomp_ret;
    (void)decomp_len;
    // API sequence test completed successfully
    return 66;
}