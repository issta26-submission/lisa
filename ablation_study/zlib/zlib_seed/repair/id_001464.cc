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
//<ID> 1464
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and write a gzip file)
    const Bytef src[] = "zlib sequence: write and read gzfile then run deflateEnd and inspect CRC table";
    uLong src_len = (uLong)(sizeof(src) - 1);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    unsigned int write_len = (unsigned int)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)src_len);
    int gw_write_ret = gzwrite(gw, (void *)src, write_len);
    int gw_close_ret = gzclose(gw);

    // step 2: Configure (open the gzip file for reading and read its contents)
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    size_t buf_size = (size_t)(src_len + 16);
    Bytef * buf = (Bytef *)malloc(buf_size);
    memset(buf, 0, (int)buf_size);
    int gzread_ret = gzread(gr, (void *)buf, (unsigned int)(buf_size - 1));
    uInt read_len = (uInt)((gzread_ret > 0) * gzread_ret);

    // step 3: Operate (initialize a deflate stream, inspect CRC table, compute CRC over read data)
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t table_first = crc_table[0];
    uLong crc_val = crc32(0UL, buf, read_len);
    uLong combined = crc_val ^ (uLong)table_first;

    // step 4: Validate & Cleanup (end deflate stream, close read gzFile, free buffer)
    int def_end_ret = deflateEnd(&dstrm);
    int gr_close_ret = gzclose_r(gr);
    free(buf);
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)gzread_ret;
    (void)read_len;
    (void)def_init_ret;
    (void)table_first;
    (void)crc_val;
    (void)combined;
    (void)def_end_ret;
    (void)gr_close_ret;
    // API sequence test completed successfully
    return 66;
}