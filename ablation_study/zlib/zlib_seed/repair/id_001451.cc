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
//<ID> 1451
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and version)
    const Bytef src[] = "zlib API sequence: deflateInit_, deflateSetHeader, deflate, inflateInit_, inflate, inflateBackInit_, inflateBackEnd";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();

    // step 2: Configure deflate stream and header, allocate output buffer
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int dinit_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (int)sizeof(head));
    head.text = 0;
    head.time = 0UL;
    head.xflags = 0;
    head.os = 255;
    int dsethdr_ret = deflateSetHeader(&dstrm, &head);
    uLong max_out = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)max_out);
    memset(comp_buf, 0, (int)max_out);
    dstrm.next_in = (Bytef *)src;
    dstrm.avail_in = (uInt)src_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)((max_out > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (uInt)max_out);
    int def_ret = deflate(&dstrm, 0);
    uLong produced = dstrm.total_out;
    int def_end_ret = deflateEnd(&dstrm);

    // step 3: Configure inflate stream and inflateBack, allocate decompression buffer
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int in_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (int)(src_len + 16));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)((produced > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (uInt)produced);
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)((src_len + 16 > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (uInt)(src_len + 16));
    int inf_ret = inflate(&istrm, 0);

    z_stream backstrm;
    memset(&backstrm, 0, (int)sizeof(backstrm));
    unsigned char window_buf[32];
    memset(window_buf, 0, (int)sizeof(window_buf));
    int back_init_ret = inflateBackInit_(&backstrm, 15, window_buf, version, (int)sizeof(z_stream));
    int back_end_ret = inflateBackEnd(&backstrm);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&istrm);
    free(comp_buf);
    free(decomp_buf);
    (void)dinit_ret;
    (void)dsethdr_ret;
    (void)def_ret;
    (void)produced;
    (void)def_end_ret;
    (void)in_init_ret;
    (void)inf_ret;
    (void)back_init_ret;
    (void)back_end_ret;
    (void)inf_end_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}