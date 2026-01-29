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
//<ID> 1450
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and allocate buffers)
    const Bytef src[] = "zlib API usage sequence: compress2 -> inflateInit_ -> inflate -> deflateInit_ -> deflateSetHeader -> inflateBackInit_";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong comp_bound = compressBound(src_len);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress2(comp, &comp_len, src, src_len, 6);
    Bytef *decomp = (Bytef *)malloc((size_t)(src_len + 64));
    memset(decomp, 0, (int)(src_len + 64));

    // step 2: Configure (initialize deflate, set gzip header, initialize inflate and inflateBack)
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header gzhead;
    memset(&gzhead, 0, (int)sizeof(gzhead));
    gzhead.name = (Bytef *)"zlib_test_name";
    gzhead.comment = (Bytef *)"zlib_test_comment";
    int def_set_hdr_ret = deflateSetHeader(&dstrm, &gzhead);

    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    z_stream backstrm;
    memset(&backstrm, 0, (int)sizeof(backstrm));
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window, 0, (int)(1 << 15));
    int inf_back_init_ret = inflateBackInit_(&backstrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate (feed compressed data into inflate and obtain decompressed output)
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)(src_len + 64);
    int inf_ret = inflate(&istrm, 0);

    // step 4: Validate & Cleanup (end streams, back-end, free buffers)
    int def_end_ret = deflateEnd(&dstrm);
    int inf_end_ret = inflateEnd(&istrm);
    int inf_back_end_ret = inflateBackEnd(&backstrm);
    free(comp);
    free(decomp);
    free(window);
    (void)comp_ret;
    (void)def_init_ret;
    (void)def_set_hdr_ret;
    (void)inf_init_ret;
    (void)inf_back_init_ret;
    (void)inf_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)inf_back_end_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}