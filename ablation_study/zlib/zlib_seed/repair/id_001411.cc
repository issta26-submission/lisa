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
//<ID> 1411
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence: compress -> deflateReset -> write gz -> gzseek64/gzrewind -> gzread -> inflateGetHeader";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf destLen = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &destLen, src, src_len, 6);
    uLong comp_len = (uLong)destLen;

    // step 2: Configure (prepare streams and headers)
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int def_reset_ret = deflateReset(&dstrm);

    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(decomp_buf, 0, (int)(src_len + 64));
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)((src_len + 64) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(src_len + 64));
    gz_header head;
    memset(&head, 0, (int)sizeof(head));
    int gethead_ret = inflateGetHeader(&istrm, &head);

    // step 3: Operate (write compressed data to a gz file, then seek, rewind and read it back)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz, comp_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int gzclose_w_ret = gzclose_w(gz);
    gz = gzopen("test_zlib_api_sequence.gz", "rb");
    off64_t seek_ret = gzseek64(gz, (off64_t)0, 0);
    int rewind_ret = gzrewind(gz);
    unsigned int read_len = (unsigned int)((comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFUL : comp_len));
    Bytef *read_buf = (Bytef *)malloc((size_t)read_len);
    memset(read_buf, 0, (int)read_len);
    int gr = gzread(gz, (voidp)read_buf, read_len);
    int gzclose_r_ret = gzclose_r(gz);

    // step 4: Validate & Cleanup
    (void)comp_ret;
    (void)def_init_ret;
    (void)def_reset_ret;
    (void)inf_init_ret;
    (void)gethead_ret;
    (void)gw;
    (void)gzclose_w_ret;
    (void)seek_ret;
    (void)rewind_ret;
    (void)gr;
    (void)gzclose_r_ret;
    uLong crc_src = crc32(0L, src, (uInt)src_len);
    uLong crc_read = crc32(0L, read_buf, (uInt)(read_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)read_len));
    (void)crc_src;
    (void)crc_read;
    int def_end_ret = deflateEnd(&dstrm);
    int inf_end_ret = inflateEnd(&istrm);
    free(comp_buf);
    free(decomp_buf);
    free(read_buf);
    // API sequence test completed successfully
    return 66;
}