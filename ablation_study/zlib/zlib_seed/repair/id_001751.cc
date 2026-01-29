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
//<ID> 1751
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (source, version, compress)
    const Bytef src[] = "zlib API sequence test: compress -> write gz -> inflateInit_ -> inflatePrime -> inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong bound = compressBound(src_len);
    uLongf comp_len = (uLongf)bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    int comp_ret = compress(comp_buf, &comp_len, src, src_len);

    // step 2: Configure (write compressed data to a gz file and capture CRC table sample)
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    off_t seek_pos = gzseek(gzf, 0, 0);
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t crc_sample = crc_tbl[0];

    // step 3: Operate (prepare inflate stream, init, prime bit buffer, inflate, check sync point)
    z_stream inf;
    memset(&inf, 0, (int)sizeof(inf));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (int)(src_len + 16));
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)(src_len + 16);
    int inf_init_ret = inflateInit_(&inf, ver, (int)sizeof(z_stream));
    int prime_ret = inflatePrime(&inf, 3, (int)(crc_sample & 0x7));
    int inf_ret = inflate(&inf, 0);
    int sync_point = inflateSyncPoint(&inf);

    // step 4: Validate & Cleanup (finalize inflate, close gz, free buffers)
    int inf_end_ret = inflateEnd(&inf);
    int gzclose_ret = gzclose(gzf);
    free(comp_buf);
    free(decomp_buf);
    (void)ver;
    (void)bound;
    (void)comp_len;
    (void)comp_ret;
    (void)gw_ret;
    (void)seek_pos;
    (void)crc_tbl;
    (void)crc_sample;
    (void)inf_init_ret;
    (void)prime_ret;
    (void)inf_ret;
    (void)sync_point;
    (void)inf_end_ret;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}