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
//<ID> 1460
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and write a gzip file)
    const Bytef src[] = "zlib api sequence: write then read and deflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_write_ret = gzwrite(gw, (void *)src, (unsigned int)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gw_close_ret = gzclose(gw);

    // step 2: Configure (open the written gzip for reading and prepare deflate stream)
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    unsigned int buf_size = 512;
    Bytef * buf = (Bytef *)malloc((size_t)buf_size);
    memset(buf, 0, (int)buf_size);
    int read_ret = gzread(gr, (void *)buf, (unsigned int)(buf_size - 1));
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    const char * version = zlibVersion();
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = buf;
    dstrm.avail_in = (uInt)(read_ret > 0 ? (unsigned int)read_ret : 0u);
    uLong out_bound = deflateBound(&dstrm, (uLong)dstrm.avail_in);
    Bytef * out_buf = (Bytef *)malloc((size_t)(out_bound + 16));
    memset(out_buf, 0, (int)(out_bound + 16));
    dstrm.next_out = out_buf;
    dstrm.avail_out = (uInt)((out_bound + 16) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)(out_bound + 16));

    // step 3: Operate (perform deflate and inspect CRC table)
    int def_ret = deflate(&dstrm, 0);
    const z_crc_t * crc_table = get_crc_table();
    z_crc_t crc_sample = crc_table ? crc_table[0] : (z_crc_t)0;

    // step 4: Validate & Cleanup (end deflate, close gzFile via gzclose_r, free buffers)
    int def_end_ret = deflateEnd(&dstrm);
    int gr_close_r_ret = gzclose_r(gr);
    free(buf);
    free(out_buf);
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)read_ret;
    (void)def_init_ret;
    (void)def_ret;
    (void)crc_sample;
    (void)def_end_ret;
    (void)gr_close_r_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}