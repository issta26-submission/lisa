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
//<ID> 826
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "The quick brown fox jumps over the lazy dog";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    uLong comp_bound = compressBound(sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)sample;
    defstrm.avail_in = (uInt)sample_len;
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)comp_bound;

    // step 2: Configure
    int def_reset_ret = deflateReset(&defstrm);
    int def_ret = deflate(&defstrm, 4);
    uLong comp_len = defstrm.total_out;
    z_stream ibstrm;
    memset(&ibstrm, 0, sizeof(ibstrm));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int ib_init_ret = inflateBackInit_(&ibstrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf = gzopen(fname, "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    off_t pos = gztell(gzf);
    int gz_cl = gzclose(gzf);

    // step 4: Validate / Cleanup
    int ib_end_ret = inflateBackEnd(&ibstrm);
    int def_end_ret = deflateEnd(&defstrm);
    free(comp_buf);
    (void)version;
    (void)sample_len;
    (void)comp_bound;
    (void)def_init_ret;
    (void)def_reset_ret;
    (void)def_ret;
    (void)comp_len;
    (void)ib_init_ret;
    (void)fname;
    (void)gz_wr;
    (void)pos;
    (void)gz_cl;
    (void)ib_end_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}