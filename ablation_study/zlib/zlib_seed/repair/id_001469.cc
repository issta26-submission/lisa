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
//<ID> 1469
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare data and initialize deflate stream)
    const Bytef src[] = "zlib sequence: write gzip, read back, compute crc, cleanup with deflateEnd";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    const char * version = zlibVersion();
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure (open gz file for write, write data, close with gzclose)
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_write_ret = gzwrite(gw, (void *)src, (unsigned int)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gw_close_ret = gzclose(gw);

    // step 3: Operate (obtain CRC table, reopen file for read, read into buffer and compute crc)
    const z_crc_t * crc_table = get_crc_table();
    uLong seed_from_table = (uLong)crc_table[0];
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    unsigned int buf_size = 1024;
    Bytef * buf = (Bytef *)malloc((size_t)buf_size);
    memset(buf, 0, (int)buf_size);
    int read_ret = gzread(gr, (void *)buf, (unsigned int)(buf_size - 1));
    uLong computed_crc = crc32(seed_from_table, buf, (uInt)(read_ret > 0 ? (unsigned int)read_ret : 0u));
    int gr_close_ret = gzclose_r(gr);

    // step 4: Validate & Cleanup (end deflate, free buffer, and tidy up)
    int def_end_ret = deflateEnd(&dstrm);
    free(buf);
    (void)version;
    (void)def_init_ret;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)crc_table;
    (void)seed_from_table;
    (void)read_ret;
    (void)computed_crc;
    (void)gr_close_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}