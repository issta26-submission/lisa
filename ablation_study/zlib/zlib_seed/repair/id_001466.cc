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
//<ID> 1466
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source and fetch crc table)
    const Bytef src[] = "zlib api sequence: write/read gz, init deflate, use crc table";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t table0 = crc_table[0];

    // step 2: Configure (initialize deflate and create gzip file for writing)
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    unsigned int write_ret = gzwrite(gw, (void *)src, (unsigned int)src_len);
    int gw_close_ret = gzclose(gw);

    // step 3: Operate (open gzip file for reading and read contents)
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *in_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(in_buf, 0, (int)(src_len + 16));
    int read_ret = gzread(gr, (void *)in_buf, (unsigned int)src_len);
    int gr_close_ret = gzclose_r(gr);

    // step 4: Validate & Cleanup (end deflate and free buffers)
    int def_end_ret = deflateEnd(&dstrm);
    free(in_buf);
    (void)table0;
    (void)def_init_ret;
    (void)write_ret;
    (void)gw_close_ret;
    (void)read_ret;
    (void)gr_close_ret;
    (void)def_end_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}