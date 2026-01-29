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
//<ID> 907
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence test payload for compression and decompression.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    int dinit_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)src_len;
    uLong out_bound = deflateBound(&dstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)out_bound;

    // step 3: Operate
    int def_ret = deflate(&dstrm, 4);
    uLong comp_len = dstrm.total_out;
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    int iinit_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)src_len);
    memset(decomp_buf, 0, (size_t)src_len);
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)src_len;
    int inflate_ret = inflate(&istrm, 0);

    // step 4: Validate / Cleanup
    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf, decomp_buf, (unsigned int)istrm.total_out);
    int gz_cl1 = gzclose(gzf);
    gzFile gzf2 = gzopen("zlib_api_sequence_tmp.gz", "rb");
    int first_char = gzgetc_(gzf2);
    int gz_cl2 = gzclose(gzf2);
    int def_end_ret = deflateEnd(&dstrm);
    int inf_end_ret = inflateEnd(&istrm);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)dinit_ret;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)iinit_ret;
    (void)inflate_ret;
    (void)gz_wr;
    (void)gz_cl1;
    (void)first_char;
    (void)gz_cl2;
    (void)def_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}