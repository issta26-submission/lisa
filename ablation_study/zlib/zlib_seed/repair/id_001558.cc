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
//<ID> 1558
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: use compress2, uncompress, inflate stream, deflate stream and check codes used";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong compile_flags = zlibCompileFlags();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 2: Configure
    Bytef *uncmp_buf = (Bytef *)malloc((size_t)src_len);
    memset(uncmp_buf, 0, (int)src_len);
    uLongf uncmp_len = src_len;
    int uncmp_ret = uncompress(uncmp_buf, &uncmp_len, comp_buf, comp_len);
    z_stream infstrm;
    memset(&infstrm, 0, (int)sizeof(infstrm));
    const char *ver = zlibVersion();
    int inf_init_ret = inflateInit_(&infstrm, ver, (int)sizeof(z_stream));
    infstrm.next_in = comp_buf;
    infstrm.avail_in = (uInt)comp_len;
    Bytef *stream_out = (Bytef *)malloc((size_t)src_len);
    memset(stream_out, 0, (int)src_len);
    infstrm.next_out = stream_out;
    infstrm.avail_out = (uInt)src_len;

    // step 3: Operate
    int inf_ret = inflate(&infstrm, 0);
    unsigned long codes_used_before_end = inflateCodesUsed(&infstrm);
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)src;
    defstrm.avail_in = (uInt)src_len;
    Bytef *def_out = (Bytef *)malloc((size_t)bound);
    memset(def_out, 0, (int)bound);
    defstrm.next_out = def_out;
    defstrm.avail_out = (uInt)bound;
    int def_ret = deflate(&defstrm, 4);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&infstrm);
    unsigned long codes_used_after_end = inflateCodesUsed(&infstrm);
    int def_end_ret = deflateEnd(&defstrm);
    free(comp_buf);
    free(uncmp_buf);
    free(stream_out);
    free(def_out);
    (void)compile_flags;
    (void)bound;
    (void)comp_len;
    (void)comp_ret;
    (void)uncmp_len;
    (void)uncmp_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)codes_used_before_end;
    (void)def_init_ret;
    (void)def_ret;
    (void)inf_end_ret;
    (void)codes_used_after_end;
    (void)def_end_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}