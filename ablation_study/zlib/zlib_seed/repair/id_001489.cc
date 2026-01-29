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
//<ID> 1489
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib sequence test: write with gzopen64, seek, read back, checksum with adler32, cleanup inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    uLong bound = deflateBound(&dstrm, src_len);
    z_stream instrm;
    memset(&instrm, 0, (int)sizeof(instrm));
    const char * version = zlibVersion();
    int inf_init_ret = inflateInit_(&instrm, version, (int)sizeof(z_stream));

    // step 2: Configure (write gzip via gzopen64 and seek)
    gzFile gw = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw_write_ret = gzwrite(gw, (void *)src, (unsigned int)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)src_len));
    off64_t gw_seek_ret = gzseek64(gw, (off64_t)0, 0);
    int gw_close_ret = gzclose(gw);

    // step 3: Operate (open with gzopen64 for read, read into buffer sized by deflateBound, then seek end)
    gzFile gr = gzopen64("test_zlib_api_sequence.gz", "rb");
    Bytef * read_buf = (Bytef *)malloc((size_t)(bound + 1UL));
    memset(read_buf, 0, (int)(bound + 1UL));
    unsigned int read_len = (unsigned int)(((bound + 1UL) > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)(bound + 1UL));
    int gr_read_ret = gzread(gr, (void *)read_buf, read_len);
    off64_t gr_seek_end = gzseek64(gr, (off64_t)0, 2);
    uLong checksum = adler32(0UL, read_buf, (uInt)(gr_read_ret > 0 ? (unsigned int)gr_read_ret : 0u));

    // step 4: Validate & Cleanup
    int gr_close_ret = gzclose(gr);
    int inf_end_ret = inflateEnd(&instrm);
    free(read_buf);
    (void)inf_init_ret;
    (void)version;
    (void)bound;
    (void)gw_write_ret;
    (void)gw_seek_ret;
    (void)gw_close_ret;
    (void)gr_read_ret;
    (void)gr_seek_end;
    (void)checksum;
    (void)gr_close_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}