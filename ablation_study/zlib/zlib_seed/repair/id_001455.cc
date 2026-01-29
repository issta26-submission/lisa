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
//<ID> 1455
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and buffers)
    const char * orig = "Zlib API sequence test data: deflateInit_, deflateSetHeader, inflateBackInit_, inflateInit_, inflate";
    uLong src_len = (uLong)strlen(orig);
    Bytef * src_buf = (Bytef *)malloc((size_t)src_len);
    memcpy(src_buf, orig, (size_t)src_len);
    const char * version = zlibVersion();
    uLong dest_cap = compressBound(src_len);
    Bytef * comp_buf = (Bytef *)malloc((size_t)dest_cap);
    memset(comp_buf, 0, (int)dest_cap);
    Bytef * out_buf = (Bytef *)malloc((size_t)src_len + 1);
    memset(out_buf, 0, (int)(src_len + 1));

    // step 2: Configure (initialize deflate stream and set gzip header, initialize inflate streams)
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (int)sizeof(head));
    head.text = 0;
    head.time = 0;
    head.xflags = 0;
    head.os = 255;
    int def_set_head_ret = deflateSetHeader(&dstrm, &head);

    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    z_stream ibstrm;
    memset(&ibstrm, 0, (int)sizeof(ibstrm));
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int inf_back_init_ret = inflateBackInit_(&ibstrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate (compress with deflate, then decompress with inflate)
    dstrm.next_in = src_buf;
    dstrm.avail_in = (uInt)src_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)dest_cap;
    int def_ret = deflate(&dstrm, 4); // Z_FINISH == 4
    uLong comp_size = dstrm.total_out;

    int def_end_ret = deflateEnd(&dstrm);

    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)src_len;
    int inf_ret = inflate(&istrm, 0); // Z_NO_FLUSH == 0

    // step 4: Validate & Cleanup (end inflate streams, free buffers)
    int inf_end_ret = inflateEnd(&istrm);
    int inf_back_end_ret = inflateBackEnd(&ibstrm);
    free(window);
    free(src_buf);
    free(comp_buf);
    free(out_buf);
    (void)def_init_ret;
    (void)def_set_head_ret;
    (void)inf_init_ret;
    (void)inf_back_init_ret;
    (void)def_ret;
    (void)comp_size;
    (void)def_end_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)inf_back_end_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}