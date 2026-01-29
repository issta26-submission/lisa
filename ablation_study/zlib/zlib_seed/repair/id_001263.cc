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
//<ID> 1263
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "zlib API sequence payload: initialize deflate, compress, write info to gz, seek and get char";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure compression buffers
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 3: Operate — compress, write info to gzip, reopen, seek and get a character
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    gzFile gzf_w = gzopen("test_zlib_api_sequence.gz", "wb");
    char info_buf[128];
    sprintf(info_buf, "zlib_ver=%s comp_size=%lu", version, (unsigned long)comp_size);
    int gz_puts_ret = gzputs(gzf_w, info_buf);
    int gzclose_w_ret = gzclose(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence.gz", "rb");
    off_t seek_off = gzseek(gzf_r, 0, 0);
    int first_char = gzgetc(gzf_r);
    int gzclose_r_ret = gzclose(gzf_r);

    // step 4: Cleanup and finalization
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    (void)def_init_ret;
    (void)def_ret;
    (void)comp_size;
    (void)gz_puts_ret;
    (void)gzclose_w_ret;
    (void)seek_off;
    (void)first_char;
    (void)gzclose_r_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}