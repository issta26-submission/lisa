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
//<ID> 1345
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Hello zlib API sequence - compress then read via gzseek/gzread/gzeof";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);

    // step 2: Configure
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);

    // step 3: Operate
    int def_ret = deflate(&strm, 4);
    uLong comp_len = (uLong)strm.total_out;
    int end_ret = deflateEnd(&strm);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz, comp_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int close_w = gzclose(gz);

    // step 4: Validate & Cleanup
    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    off_t seek_pos = gzseek(gz_r, (off_t)0, 0);
    Bytef *read_buf = (Bytef *)malloc((size_t)(comp_len + 1));
    memset(read_buf, 0, (int)(comp_len + 1));
    int gr = gzread(gz_r, read_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int eof = gzeof(gz_r);
    int close_r = gzclose(gz_r);
    free(comp_buf);
    free(read_buf);

    (void)init_ret;
    (void)def_ret;
    (void)end_ret;
    (void)gw;
    (void)close_w;
    (void)seek_pos;
    (void)gr;
    (void)eof;
    (void)close_r;
    (void)version;
    // API sequence test completed successfully
    return 66;
}