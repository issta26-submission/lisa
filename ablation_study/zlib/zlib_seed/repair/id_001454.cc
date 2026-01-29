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
//<ID> 1454
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and warm up deflate with a gz header)
    const Bytef src[] = "zlib API sequence: deflateInit_, deflateSetHeader, compress, inflateInit_, inflateBackInit_, inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int dinit_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (int)sizeof(head));
    head.text = 0;
    head.time = 0UL;
    head.os = 255;
    int sethead_ret = deflateSetHeader(&dstrm, &head);

    // step 2: Configure (compress source with compress() into a buffer, and prepare inflate streams)
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress(comp_buf, &comp_len, src, src_len);
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int ininit_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    z_stream bstrm;
    memset(&bstrm, 0, (int)sizeof(bstrm));
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window, 0, (int)(1 << 15));
    int backinit_ret = inflateBackInit_(&bstrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate (feed compressed data to inflate and perform inflation)
    Bytef *out_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(out_buf, 0, (int)(src_len + 16));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(src_len + 16);
    int inflate_ret = inflate(&istrm, 0);

    // step 4: Validate & Cleanup (end streams and free resources)
    int defend_ret = deflateEnd(&dstrm);
    int infend_ret = inflateEnd(&istrm);
    int backend_ret = inflateBackEnd(&bstrm);
    free(comp_buf);
    free(out_buf);
    free(window);
    (void)dinit_ret;
    (void)sethead_ret;
    (void)comp_ret;
    (void)ininit_ret;
    (void)backinit_ret;
    (void)inflate_ret;
    (void)defend_ret;
    (void)infend_ret;
    (void)backend_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}