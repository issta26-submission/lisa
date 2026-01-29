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
//<ID> 1483
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib sequence: open with gzopen64, write, seek with gzseek64, read, use deflateBound and inflateInit_ for sizing and init";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    memset(&istrm, 0, (int)sizeof(istrm));
    const char * version = zlibVersion();
    int infl_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, src_len);
    Bytef * buf = (Bytef *)malloc((size_t)((bound == 0UL) ? 1UL : bound));
    memset(buf, 0, (int)((bound == 0UL) ? 1 : (int)bound));

    // step 2: Configure (write gzip file using 64-bit API)
    gzFile gw = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw_write_ret = gzwrite(gw, (void *)src, (unsigned int)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gw_close_ret = gzclose(gw);

    // step 3: Operate (open for read with gzopen64, seek with gzseek64, read into buffer)
    gzFile gr = gzopen64("test_zlib_api_sequence.gz", "rb");
    off64_t seek_ret = gzseek64(gr, (off64_t)0, 0);
    int read_ret = gzread(gr, (void *)buf, (unsigned int)((bound > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)bound));

    // step 4: Validate & Cleanup
    int gr_close_ret = gzclose(gr);
    int def_end_ret = deflateEnd(&dstrm);
    int infl_end_ret = inflateEnd(&istrm);
    free(buf);
    (void)version;
    (void)infl_init_ret;
    (void)bound;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)seek_ret;
    (void)read_ret;
    (void)gr_close_ret;
    (void)def_end_ret;
    (void)infl_end_ret;
    // API sequence test completed successfully
    return 66;
}