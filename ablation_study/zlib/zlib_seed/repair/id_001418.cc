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
//<ID> 1418
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence test: write gz, rewind, read, deflate/reset, inspect header";
    uLong src_len = (uLong)(sizeof(src) - 1);
    gzFile gwfile = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    unsigned int write_len = (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len);
    int gw_ret = gzwrite(gwfile, src, write_len);
    int gw_close_ret = gzclose(gwfile);

    // step 2: Configure
    gzFile grfile = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int dinit_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int dreset_ret = deflateReset(&dstrm);
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int iinit_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    gz_header header;
    memset(&header, 0, (int)sizeof(header));
    int igethead_ret = inflateGetHeader(&istrm, &header);
    int grewind_ret = gzrewind(grfile);
    off64_t seek_ret = gzseek64(grfile, (off64_t)0, 0);

    // step 3: Operate
    Bytef *read_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(read_buf, 0, (int)(src_len + 16));
    int gread_ret = gzread(grfile, read_buf, write_len);
    dstrm.next_in = read_buf;
    dstrm.avail_in = (uInt)(write_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)write_len);
    uLong out_bound = deflateBound(&dstrm, (uLong)dstrm.avail_in);
    Bytef *out_buf = (Bytef *)malloc((size_t)(out_bound + 8));
    memset(out_buf, 0, (int)(out_bound + 8));
    dstrm.next_out = out_buf;
    dstrm.avail_out = (uInt)((out_bound + 8) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(out_bound + 8));
    int def_ret = deflate(&dstrm, 0);

    // step 4: Validate & Cleanup
    int dend_ret = deflateEnd(&dstrm);
    int iend_ret = inflateEnd(&istrm);
    int gr_close_ret = gzclose(grfile);
    uLong crc_read = crc32(0L, read_buf, (uInt)(write_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)write_len));
    (void)gw_ret;
    (void)gw_close_ret;
    (void)dinit_ret;
    (void)dreset_ret;
    (void)iinit_ret;
    (void)igethead_ret;
    (void)grewind_ret;
    (void)seek_ret;
    (void)gread_ret;
    (void)def_ret;
    (void)dend_ret;
    (void)iend_ret;
    (void)gr_close_ret;
    (void)crc_read;
    (void)version;
    free(read_buf);
    free(out_buf);
    // API sequence test completed successfully
    return 66;
}