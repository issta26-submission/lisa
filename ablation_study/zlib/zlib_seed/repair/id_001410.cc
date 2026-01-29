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
//<ID> 1410
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence test: write gz -> seek64/rewind/read -> inflateGetHeader -> deflateReset";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf destLen = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &destLen, src, src_len, 6);
    uLong comp_len = (uLong)destLen;

    // step 2: Configure
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int def_reset_ret = deflateReset(&dstrm);
    gzFile gzw = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gzbuf_ret = gzbuffer(gzw, 8192);
    int gw = gzwrite(gzw, comp_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int gzclose_ret_w = gzclose(gzw);

    // step 3: Operate
    gzFile gzr = gzopen64("test_zlib_api_sequence.gz", "rb");
    off64_t seek_ret = gzseek64(gzr, 0, 0);
    int rewind_ret = gzrewind(gzr);
    Bytef *read_buf = (Bytef *)malloc((size_t)(comp_len + 8));
    memset(read_buf, 0, (int)(comp_len + 8));
    int bytes_read = gzread(gzr, read_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));

    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit2_(&istrm, 47, version, (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)(bytes_read > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bytes_read);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(decomp_buf, 0, (int)(src_len + 64));
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)((src_len + 64) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(src_len + 64));
    gz_header head;
    memset(&head, 0, (int)sizeof(head));
    int hdr_ret = inflateGetHeader(&istrm, &head);
    int inf_ret = inflate(&istrm, 0);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&istrm);
    int def_end_ret = deflateEnd(&dstrm);
    int gzclose_ret_r = gzclose(gzr);
    (void)comp_ret;
    (void)def_init_ret;
    (void)def_reset_ret;
    (void)gzbuf_ret;
    (void)gw;
    (void)gzclose_ret_w;
    (void)seek_ret;
    (void)rewind_ret;
    (void)bytes_read;
    (void)inf_init_ret;
    (void)hdr_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    (void)gzclose_ret_r;
    (void)version;
    free(comp_buf);
    free(read_buf);
    free(decomp_buf);
    // API sequence test completed successfully
    return 66;
}