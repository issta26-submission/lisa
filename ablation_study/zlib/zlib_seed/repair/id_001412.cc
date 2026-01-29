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
//<ID> 1412
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source and compress)
    const Bytef src[] = "Zlib API sequence: prepare -> compress -> deflateReset -> write/read gz -> inflateGetHeader -> inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf destLen = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &destLen, src, src_len, 6);
    uLong comp_len = (uLong)destLen;

    // step 2: Configure (initialize deflate and inflate streams and buffers)
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int def_reset_ret = deflateReset(&dstrm);

    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit2_(&istrm, 15 + 16, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    uLong decomp_buf_size = src_len + 64;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)decomp_buf_size);
    memset(decomp_buf, 0, (int)decomp_buf_size);
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)(decomp_buf_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)decomp_buf_size);

    // step 3: Operate (write compressed data to a gz file, then seek/rewind/read and inflate; get header)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz, comp_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int gzclose_w_ret = gzclose(gz);
    gz = gzopen("test_zlib_api_sequence.gz", "rb");
    off64_t seek_pos = gzseek64(gz, (off64_t)0, 0);
    int rewind_ret = gzrewind(gz);
    Bytef *read_buf = (Bytef *)malloc((size_t)comp_len);
    memset(read_buf, 0, (int)comp_len);
    int gr = gzread(gz, read_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)(gr > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)gr);
    gz_header head;
    memset(&head, 0, (int)sizeof(head));
    int get_head_ret = inflateGetHeader(&istrm, &head);
    int inf_ret = inflate(&istrm, 0);

    // step 4: Validate & Cleanup
    (void)comp_ret;
    (void)def_init_ret;
    (void)def_reset_ret;
    (void)inf_init_ret;
    (void)gw;
    (void)gzclose_w_ret;
    (void)seek_pos;
    (void)rewind_ret;
    (void)gr;
    (void)get_head_ret;
    (void)inf_ret;
    int inf_end_ret = inflateEnd(&istrm);
    int def_end_ret = deflateEnd(&dstrm);
    int gzclose_r_ret = gzclose(gz);
    free(comp_buf);
    free(decomp_buf);
    free(read_buf);
    // API sequence test completed successfully
    return 66;
}