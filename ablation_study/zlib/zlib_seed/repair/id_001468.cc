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
//<ID> 1468
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and initialize deflate stream)
    const Bytef src[] = "zlib sequence: prepare data, deflate once, write/read via gzopen/gzclose variants";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char * version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure (allocate output buffer using deflateBound and perform a deflate)
    uLong comp_bound = deflateBound(&dstrm, src_len);
    Bytef * comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    dstrm.next_in = (Bytef *)src;
    dstrm.avail_in = (uInt)src_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)comp_bound;
    int def_ret = deflate(&dstrm, Z_FINISH);
    uLong comp_len = dstrm.total_out;

    // step 3: Operate (write original data to a gzip file, then reopen and read it back)
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_write_ret = gzwrite(gw, (void *)src, (unsigned int)src_len);
    int gw_close_ret = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    unsigned int read_buf_size = (unsigned int)(src_len + 16);
    Bytef * read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (int)read_buf_size);
    int read_ret = gzread(gr, (void *)read_buf, read_buf_size);
    const z_crc_t * crc_table = get_crc_table();
    z_crc_t crc0 = crc_table[0];
    int gr_close_r_ret = gzclose_r(gr);

    // step 4: Validate & Cleanup (end deflate stream, free buffers, and tidy up)
    int def_end_ret = deflateEnd(&dstrm);
    free(comp_buf);
    free(read_buf);
    (void)version;
    (void)def_init_ret;
    (void)def_ret;
    (void)comp_len;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)read_ret;
    (void)crc0;
    (void)gr_close_r_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}