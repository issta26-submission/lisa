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
//<ID> 1415
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence test: prepare compressed data to exercise gzseek64, gzrewind, deflateReset, gzread, inflateGetHeader";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_destLen = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_destLen, src, src_len, 6);
    uLong comp_len = (uLong)comp_destLen;

    // step 2: Configure
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int def_reset_ret = deflateReset(&dstrm);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_bytes = gzwrite(gw, comp_buf, (unsigned int)comp_len);
    int gw_close_ret = gzclose(gw);
    (void)comp_ret;
    (void)def_init_ret;
    (void)def_reset_ret;
    (void)gw_bytes;
    (void)gw_close_ret;

    // step 3: Operate
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    off64_t pos = gzseek64(gr, (off64_t)0, 0);
    Bytef *read_buf = (Bytef *)malloc((size_t)comp_len);
    memset(read_buf, 0, (int)comp_len);
    int gr_bytes = gzread(gr, (voidp)read_buf, (unsigned int)comp_len);
    int gr_rewind_ret = gzrewind(gr);

    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    gz_header header;
    memset(&header, 0, (int)sizeof(header));
    int gh_ret = inflateGetHeader(&istrm, &header);
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)(gr_bytes < 0 ? 0 : (uInt)gr_bytes);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (int)(src_len + 16));
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)(src_len + 16);
    int inf_ret = inflate(&istrm, 0);

    // step 4: Validate & Cleanup
    (void)pos;
    (void)gh_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    int inf_end_ret = inflateEnd(&istrm);
    int def_end_ret = deflateEnd(&dstrm);
    int gr_close_ret = gzclose(gr);
    free(comp_buf);
    free(read_buf);
    free(decomp_buf);
    (void)inf_end_ret;
    (void)def_end_ret;
    (void)gr_close_ret;
    // API sequence test completed successfully
    return 66;
}