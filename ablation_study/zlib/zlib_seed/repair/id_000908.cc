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
//<ID> 908
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API fuzzing payload for compression/inflation test.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    int dinit_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&dstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&dstrm, 4);
    uLong comp_len = dstrm.total_out;
    int dend_ret = deflateEnd(&dstrm);
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    int iinit_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len;
    uLong decomp_buf_size = src_len + 64;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)decomp_buf_size);
    memset(decomp_buf, 0, (size_t)decomp_buf_size);
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)decomp_buf_size;
    int inf_ret = inflate(&istrm, 4);
    uLong decomp_len = istrm.total_out;
    int iend_ret = inflateEnd(&istrm);

    // step 4: Validate / Cleanup
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_cl1 = gzclose(gzf);
    gzFile gzf2 = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int ch = gzgetc_(gzf2);
    int gz_cl2 = gzclose(gzf2);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)dinit_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)dend_ret;
    (void)iinit_ret;
    (void)inf_ret;
    (void)decomp_len;
    (void)iend_ret;
    (void)gz_cl1;
    (void)ch;
    (void)gz_cl2;
    // API sequence test completed successfully
    return 66;
}