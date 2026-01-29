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
//<ID> 1467
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data, write a gzip file using gzopen + gzwrite, close with gzclose)
    const Bytef src[] = "zlib API sequence: write gzip, read back, compute crc, deflate init/end";
    uLong src_len = (uLong)(sizeof(src) - 1);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_write_ret = gzwrite(gw, (void *)src, (unsigned int)src_len);
    int gw_close_ret = gzclose(gw);

    // step 2: Configure (open the written gzip for reading, read contents, compute CRC, fetch CRC table)
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    unsigned int rbuf_size = (unsigned int)(src_len + 16);
    Bytef * rbuf = (Bytef *)malloc((size_t)rbuf_size);
    memset(rbuf, 0, (int)rbuf_size);
    int gr_read_ret = gzread(gr, (void *)rbuf, rbuf_size);
    uLong crc_read = crc32(0UL, rbuf, (uInt)(gr_read_ret > 0 ? (unsigned int)gr_read_ret : 0u));
    const z_crc_t * crc_table = get_crc_table();
    z_crc_t crc_table_first = crc_table[0];

    // step 3: Operate (initialize a deflate stream, point it at read buffer, perform a deflate call into a comp buffer)
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    const char * version = zlibVersion();
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = rbuf;
    dstrm.avail_in = (uInt)(gr_read_ret > 0 ? (unsigned int)gr_read_ret : 0u);
    uLong comp_bound = compressBound((uLong)dstrm.avail_in);
    Bytef * comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)comp_bound;
    int def_ret = deflate(&dstrm, 0);

    // step 4: Validate & Cleanup (end deflate stream with deflateEnd, close gzFile with gzclose_r, free buffers)
    int def_end_ret = deflateEnd(&dstrm);
    int gr_close_ret = gzclose_r(gr);
    free(comp_buf);
    free(rbuf);
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)gr_read_ret;
    (void)crc_read;
    (void)crc_table_first;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)gr_close_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}