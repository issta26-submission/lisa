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
//<ID> 1273
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "zlib API sequence payload: deflateTune and deflateSetHeader, write with gzopen64 and observe position with gztell64";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit2_(&def_strm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));

    // step 2: Configure compression buffers and header/tune
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    gz_header gh;
    memset(&gh, 0, (int)sizeof(gh));
    gh.name = (Bytef *)"test-name";
    gh.name_max = (uInt)8;
    gh.comment = (Bytef *)"comment";
    gh.comm_max = (uInt)7;
    int set_head_ret = deflateSetHeader(&def_strm, &gh);
    int tune_ret = deflateTune(&def_strm, 16, 8, 64, 128);

    // step 3: Operate - deflate, write to gz (64-bit), get position
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gzf, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    off64_t gz_pos64 = gztell64(gzf);
    uLong src_adler = adler32(1UL, source, (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len));

    // step 4: Validate (implicit) and cleanup
    int def_end_ret = deflateEnd(&def_strm);
    int gzclose_ret = gzclose(gzf);
    free(comp_buf);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)set_head_ret;
    (void)tune_ret;
    (void)def_ret;
    (void)comp_size;
    (void)gz_write_ret;
    (void)gz_pos64;
    (void)src_adler;
    (void)def_end_ret;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}