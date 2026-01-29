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
//<ID> 1555
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: use zlibCompileFlags, compress2 -> inflate (stream) -> uncompress (one-shot) -> deflateEnd/inflateEnd checks";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong compile_flags = zlibCompileFlags();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 2: Configure
    z_stream infstream;
    memset(&infstream, 0, (int)sizeof(infstream));
    const char *ver = zlibVersion();
    int inf_init_ret = inflateInit_(&infstream, ver, (int)sizeof(z_stream));
    infstream.next_in = comp_buf;
    infstream.avail_in = (uInt)comp_len;
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    infstream.next_out = out_buf;
    infstream.avail_out = (uInt)src_len;

    // step 3: Operate
    int inf_ret = inflate(&infstream, 0);
    unsigned long codes_used = inflateCodesUsed(&infstream);
    Bytef *verify_buf = (Bytef *)malloc((size_t)src_len);
    memset(verify_buf, 0, (int)src_len);
    uLongf verify_len = (uLongf)src_len;
    int unret = uncompress(verify_buf, &verify_len, comp_buf, comp_len);
    z_stream defstream;
    memset(&defstream, 0, (int)sizeof(defstream));
    int def_init_ret = deflateInit_(&defstream, 6, ver, (int)sizeof(z_stream));

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&infstream);
    int def_end_ret = deflateEnd(&defstream);
    free(comp_buf);
    free(out_buf);
    free(verify_buf);
    (void)compile_flags;
    (void)bound;
    (void)comp_len;
    (void)comp_ret;
    (void)ver;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)verify_len;
    (void)unret;
    (void)def_init_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}