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
//<ID> 1261
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef src[] = "zlib sequence: compress then write to .gz, reopen, seek and read first byte";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure compression buffers and perform deflate
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;

    // step 3: Write compressed data and a text marker to gzip, then close and reopen for reading and seeking
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gzf, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    int gz_puts_ret = gzputs(gzf, " --marker--");
    int gz_close_ret1 = gzclose(gzf);
    gzFile gzf_r = gzopen("test_zlib_api_sequence.gz", "rb");
    off_t seek_pos = gzseek(gzf_r, (off_t)0, 0);
    int first_char = gzgetc(gzf_r);
    int gz_close_ret2 = gzclose(gzf_r);

    // step 4: Cleanup and finalization
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_size;
    (void)gz_write_ret;
    (void)gz_puts_ret;
    (void)gz_close_ret1;
    (void)seek_pos;
    (void)first_char;
    (void)gz_close_ret2;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}