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
//<ID> 1465
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and compute initial CRC)
    const Bytef src[] = "zlib API sequence sample data";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong crc_start = crc32(0UL, src, (uInt)src_len);

    // step 2: Configure (create a gzip file and write compressed data)
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_write_ret = gzwrite(gw, (void *)src, (unsigned int)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gw_flush_ret = gzflush(gw, 0);
    int gw_close_ret = gzclose(gw);

    // step 3: Operate (read the gzip file back and update CRC)
    Bytef *buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(buf, 0, (int)(src_len + 16));
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    int gr_read_ret = gzread(gr, (void *)buf, (unsigned int)(src_len + 15));
    uLong crc_after = crc32(crc_start, buf, (uInt)(gr_read_ret > 0 ? (unsigned int)gr_read_ret : 0u));
    int gr_close_r_ret = gzclose_r(gr);

    // step 4: Validate & Cleanup (inspect CRC table and properly end a deflate stream)
    const z_crc_t *crc_table = get_crc_table();
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    int def_end_ret = deflateEnd(&strm);
    free(buf);
    (void)gw_write_ret;
    (void)gw_flush_ret;
    (void)gw_close_ret;
    (void)gr_read_ret;
    (void)crc_after;
    (void)gr_close_r_ret;
    (void)crc_table;
    (void)def_init_ret;
    (void)def_end_ret;
    (void)crc_start;
    (void)src_len;
    (void)version;
    // API sequence test completed successfully
    return 66;
}