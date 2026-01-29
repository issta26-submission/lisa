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
//<ID> 1722
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence: deflate -> inflate -> crc32 -> gzwrite/gzseek64/gzoffset";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    memset(&istrm, 0, (int)sizeof(istrm));
    int dinit_ret = deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    int iinit_ret = inflateInit_(&istrm, ver, (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = deflateBound(&dstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (int)(src_len + 16));
    dstrm.next_in = (Bytef *)src;
    dstrm.avail_in = (uInt)src_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&dstrm, 4);
    uLong comp_len = dstrm.total_out;
    int dend_ret = deflateEnd(&dstrm);
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)(src_len + 16);
    int inf_ret = inflate(&istrm, 4);
    uLong decomp_len = istrm.total_out;
    if (decomp_len < (uLong)(src_len + 16)) {
        decomp_buf[decomp_len] = (Byte)'\0';
    } else {
        decomp_buf[(src_len + 15)] = (Byte)'\0';
    }
    uLong checksum = crc32(0UL, decomp_buf, (uInt)decomp_len);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf, decomp_buf, (unsigned int)decomp_len);
    off_t offset_before = gzoffset(gzf);
    off64_t seek_ret = gzseek64(gzf, 0LL, 0);
    off_t offset_after = gzoffset(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    int iend_ret = inflateEnd(&istrm);
    free(comp_buf);
    free(decomp_buf);
    (void)ver;
    (void)dinit_ret;
    (void)iinit_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)dend_ret;
    (void)inf_ret;
    (void)decomp_len;
    (void)checksum;
    (void)gw_ret;
    (void)offset_before;
    (void)seek_ret;
    (void)offset_after;
    (void)gzclose_ret;
    (void)iend_ret;
    // API sequence test completed successfully
    return 66;
}