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
//<ID> 1459
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and initialize deflate with a gz header)
    const Bytef src[] = "zlib API sequence: deflateInit_, deflateSetHeader, compress2, inflateInit_, inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (int)sizeof(head));
    head.name = (Bytef *)"zlib-name";
    head.name_max = (uInt)9;
    int def_set_head_ret = deflateSetHeader(&dstrm, &head);

    // step 2: Configure (compress source with compress2 into a buffer)
    uLong comp_bound = compressBound(src_len);
    Bytef * comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 3: Operate (initialize inflate, decompress into out buffer, and initialize inflateBack for separate stream)
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    Bytef * out_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(out_buf, 0, (int)(src_len + 16));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(src_len + 16);
    int inf_ret = inflate(&istrm, 0);

    z_stream backstrm;
    memset(&backstrm, 0, (int)sizeof(backstrm));
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int back_init_ret = inflateBackInit_(&backstrm, 15, window, version, (int)sizeof(z_stream));
    int back_end_ret = inflateBackEnd(&backstrm);

    // step 4: Validate & Cleanup (end streams and free buffers)
    int def_end_ret = deflateEnd(&dstrm);
    int inf_end_ret = inflateEnd(&istrm);
    free(comp_buf);
    free(out_buf);
    free(window);
    (void)def_init_ret;
    (void)def_set_head_ret;
    (void)comp_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)back_init_ret;
    (void)back_end_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}