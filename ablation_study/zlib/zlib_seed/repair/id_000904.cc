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
//<ID> 904
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib payload for compress/decompress with gz file.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    int dinit = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&dstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)bound;

    // step 2: Operate compression and write gz file
    int ddef = deflate(&dstrm, 4);
    uLong comp_len = dstrm.total_out;
    int dend = deflateEnd(&dstrm);
    gzFile gzw = gzopen("zlib_test_tmp.gz", "wb");
    int gw1 = gzwrite(gzw, source, (unsigned int)src_len);
    int gw2 = gzwrite(gzw, comp_buf, (unsigned int)comp_len);
    int gzwc = gzclose(gzw);

    // step 3: Read back from gz file and inspect with gzgetc_
    gzFile gzr = gzopen("zlib_test_tmp.gz", "rb");
    int first_char = gzgetc_(gzr);
    int gzr_close = gzclose(gzr);

    // step 4: Decompress using inflate and cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    int iinit = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len;
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len + 16);
    memset(out_buf, 0, (size_t)src_len + 16);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(src_len + 16);
    int iret = inflate(&istrm, 0);
    uLong decomp_len = istrm.total_out;
    int iend = inflateEnd(&istrm);

    free(comp_buf);
    free(out_buf);
    (void)version; (void)dinit; (void)ddef; (void)dend; (void)gw1; (void)gw2; (void)gzwc;
    (void)first_char; (void)gzr_close; (void)iinit; (void)iret; (void)decomp_len; (void)iend;

    // API sequence test completed successfully
    return 66;
}