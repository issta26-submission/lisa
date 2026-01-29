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
//<ID> 1264
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef src[] = "zlib API sequence payload: compress, write compressed bytes to gz, then reopen and read a byte";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure compression buffers and perform deflate
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int def_ret = deflate(&strm, 4);
    uLong comp_size = strm.total_out;
    int def_end_ret = deflateEnd(&strm);

    // step 3: Write version string and compressed data to gz, then reopen and seek/read a byte
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gzputs_ret = gzputs(gzf, version);
    int gzwrite_ret = gzwrite(gzf, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    int gzclose_ret = gzclose(gzf);
    gzFile gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    off_t gzseek_ret = gzseek(gzr, 0, 0);
    int first_char = gzgetc(gzr);
    int gzclose_r_ret = gzclose(gzr);

    // step 4: Cleanup and finalization
    free(comp_buf);
    (void)init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)bound;
    (void)gzputs_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)gzseek_ret;
    (void)first_char;
    (void)gzclose_r_ret;
    // API sequence test completed successfully
    return 66;
}