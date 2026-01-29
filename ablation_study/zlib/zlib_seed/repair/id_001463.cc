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
//<ID> 1463
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and create a gzip file)
    const Bytef src[] = "zlib API sequence: write/read, use crc table, init/end deflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_write_ret = gzwrite(gw, (void *)src, (unsigned int)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gw_close_ret = gzclose(gw);

    // step 2: Configure (open the gzip for reading and obtain CRC table)
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t tbl0 = crc_tbl[0];
    int first_char = gzgetc(gr);

    // step 3: Operate (compute a crc based on table content, initialize deflate, prepare output buffer)
    uLong computed_crc = crc32((uLong)tbl0, src, (uInt)src_len);
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong out_bound = compressBound(src_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (int)out_bound);
    dstrm.next_in = (Bytef *)src;
    dstrm.avail_in = (uInt)src_len;
    dstrm.next_out = out_buf;
    dstrm.avail_out = (uInt)out_bound;
    int def_end_ret = deflateEnd(&dstrm);

    // step 4: Validate & Cleanup (close the reader with gzclose_r, free buffers, finalize)
    int gr_close_ret = gzclose_r(gr);
    free(out_buf);
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)crc_tbl;
    (void)first_char;
    (void)computed_crc;
    (void)def_init_ret;
    (void)def_end_ret;
    (void)version;
    (void)gr_close_ret;
    // API sequence test completed successfully
    return 66;
}