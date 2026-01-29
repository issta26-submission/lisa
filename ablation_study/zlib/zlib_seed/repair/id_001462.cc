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
//<ID> 1462
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and obtain CRC table)
    const Bytef src[] = "zlib sequence: write gz, read gz, compute crc, init/end deflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const z_crc_t * crc_table = get_crc_table();
    z_crc_t seed_crc = crc_table[0];

    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_write_ret = gzwrite(gw, (void *)src, (unsigned int)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gw_close_ret = gzclose(gw);

    // step 2: Configure (open for reading and prepare buffer)
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    unsigned int buf_size = (unsigned int)(src_len + 16);
    Bytef * buf = (Bytef *)malloc((size_t)buf_size);
    memset(buf, 0, (int)buf_size);
    int gz_read_ret = gzread(gr, (void *)buf, (unsigned int)(buf_size - 1));
    unsigned int read_len = (unsigned int)((gz_read_ret > 0) ? (unsigned int)gz_read_ret : 0u);
    uLong computed_crc = crc32(0UL, buf, (uInt)read_len);

    // step 3: Operate (initialize and immediately end a deflate stream to exercise deflateEnd)
    const char * version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int def_end_ret = deflateEnd(&dstrm);

    // step 4: Validate & Cleanup (close read gzFile with gzclose_r and free resources)
    int gr_close_ret = gzclose_r(gr);
    free(buf);
    (void)seed_crc;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)gz_read_ret;
    (void)computed_crc;
    (void)def_init_ret;
    (void)def_end_ret;
    (void)gr_close_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}