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
//<ID> 1461
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare data, write gzip file, obtain CRC table)
    const Bytef src[] = "zlib sequence: write gzip file, then read back and checksum";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t crc_seed = crc_table[0];
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_write_ret = gzwrite(gw, (void *)src, (unsigned int)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gw_close_ret = gzclose(gw);

    // step 2: Configure (initialize a deflate stream and then end it to exercise deflateEnd)
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    int def_end_ret = deflateEnd(&dstrm);

    // step 3: Operate (open the gzip file for reading, read into buffer, compute crc32 of read bytes)
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    unsigned int buf_size = 1024;
    Bytef *buf = (Bytef *)malloc((size_t)buf_size);
    memset(buf, 0, (int)buf_size);
    int read_ret = gzread(gr, (void *)buf, (unsigned int)(buf_size - 1));
    uLong computed_crc = crc32(0UL, buf, (uInt)(read_ret > 0 ? (unsigned int)read_ret : 0u));

    // step 4: Validate & Cleanup (close reader via gzclose_r and free buffer)
    int gr_close_ret = gzclose_r(gr);
    free(buf);
    (void)src_len;
    (void)crc_seed;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)def_init_ret;
    (void)def_end_ret;
    (void)read_ret;
    (void)computed_crc;
    (void)gr_close_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}