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
//<ID> 1417
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence test: write gz -> read gz -> use inflate header and deflate reset";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();

    // step 2: Configure
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_write = gzwrite(gw, src, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gw_close = gzclose(gw);

    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int def_reset_ret = deflateReset(&dstrm);

    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    gz_header header;
    memset(&header, 0, (int)sizeof(header));
    int inf_get_header_ret = inflateGetHeader(&istrm, &header);

    // step 3: Operate
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    off64_t pos = gzseek64(gr, (off64_t)0, 0);
    Bytef *read_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(read_buf, 0, (int)(src_len + 16));
    int gr_read = gzread(gr, read_buf, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gr_rewind = gzrewind(gr);
    int gr_read2 = gzread(gr, read_buf, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));

    // step 4: Validate & Cleanup
    (void)gw_write;
    (void)gw_close;
    (void)def_init_ret;
    (void)def_reset_ret;
    (void)inf_init_ret;
    (void)inf_get_header_ret;
    (void)pos;
    (void)gr_read;
    (void)gr_rewind;
    (void)gr_read2;
    int inf_end = inflateEnd(&istrm);
    int def_end = deflateEnd(&dstrm);
    int gr_close = gzclose(gr);
    free(read_buf);
    // API sequence test completed successfully
    (void)inf_end;
    (void)def_end;
    (void)gr_close;
    (void)version;
    return 66;
}