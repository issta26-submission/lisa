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
//<ID> 570
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef input_buf[256];
    memset(input_buf, 'A', sizeof(input_buf));
    Bytef compressed_buf[512];
    memset(compressed_buf, 0, sizeof(compressed_buf));
    uLongf comp_len = (uLongf)sizeof(compressed_buf);
    int comp_ret = compress(compressed_buf, &comp_len, input_buf, (uLong)sizeof(input_buf));

    // step 2: Setup / Configure
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzFile gw64 = gzopen64("test_zlib_api_sequence64.tmp.gz", "wb");
    off_t pos_start_gw = gzseek(gw, (off_t)0, 0);
    off_t pos_start_gw64 = gzseek(gw64, (off_t)0, 0);

    // step 3: Operate
    int write1 = gzwrite(gw, compressed_buf, (unsigned int)comp_len);
    off_t pos_after_write = gzseek(gw, (off_t)comp_len, 1);
    int write2 = gzwrite(gw64, input_buf, (unsigned int)sizeof(input_buf));
    off_t pos_end_gw64 = gzseek(gw64, (off_t)0, 2);

    // step 4: Validate / Cleanup
    int close_gw = gzclose(gw);
    int close_gw64 = gzclose(gw64);
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    Bytef read_back[512];
    memset(read_back, 0, sizeof(read_back));
    int read_ret = gzread(gr, read_back, (unsigned int)sizeof(read_back));
    int close_gr = gzclose(gr);

    (void)ver;
    (void)comp_ret;
    (void)comp_len;
    (void)pos_start_gw;
    (void)pos_start_gw64;
    (void)write1;
    (void)pos_after_write;
    (void)write2;
    (void)pos_end_gw64;
    (void)close_gw;
    (void)close_gw64;
    (void)read_ret;
    (void)close_gr;
    // API sequence test completed successfully
    return 66;
}