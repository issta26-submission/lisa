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
//<ID> 1453
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and compute a compressed buffer)
    const Bytef src[] = "zlib API sequence test: compress2 -> inflateInit_ -> inflate -> inflateBackInit_ -> deflateInit_ -> deflateSetHeader";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 2: Configure (initialize deflate stream with header, init inflate streams including inflateBack)
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (int)sizeof(head));
    static Bytef head_name[] = "zlib-name";
    static Bytef head_comment[] = "zlib-comment";
    head.name = head_name;
    head.name_max = (uInt)(sizeof(head_name) - 1);
    head.comment = head_comment;
    head.comm_max = (uInt)(sizeof(head_comment) - 1);
    int def_set_head_ret = deflateSetHeader(&dstrm, &head);

    z_stream bstrm;
    memset(&bstrm, 0, (int)sizeof(bstrm));
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window, 0, (int)(1 << 15));
    int infl_back_init_ret = inflateBackInit_(&bstrm, 15, window, version, (int)sizeof(z_stream));

    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int infl_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Operate (inflate the compressed buffer into an output buffer)
    uLong out_buf_size = src_len + 64;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_buf_size);
    memset(out_buf, 0, (int)out_buf_size);
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)out_buf_size;
    int infl_ret = inflate(&istrm, 0);

    // step 4: Validate & Cleanup (end/cleanup streams and free resources)
    int def_end_ret = deflateEnd(&dstrm);
    int infl_end_ret = inflateEnd(&istrm);
    int infl_back_end_ret = inflateBackEnd(&bstrm);
    free(comp_buf);
    free(out_buf);
    free(window);
    (void)comp_ret;
    (void)def_init_ret;
    (void)def_set_head_ret;
    (void)infl_back_init_ret;
    (void)infl_init_ret;
    (void)infl_ret;
    (void)def_end_ret;
    (void)infl_end_ret;
    (void)infl_back_end_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}