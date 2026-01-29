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
//<ID> 990
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence: deflate into buffer, query pending bits, consult CRC table, and write a byte with gzdopen.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    (void)init_ret;

    // step 2: Configure
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&strm, 0);
    unsigned int pending = 0;
    int bits = 0;
    int pend_ret = deflatePending(&strm, &pending, &bits);
    uLong comp_size = strm.total_out;
    int end_ret = deflateEnd(&strm);
    (void)def_ret;
    (void)pend_ret;
    (void)end_ret;

    // step 4: Validate / Cleanup
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl[0];
    gzFile gzf = gzdopen(1, "wb");
    int gz_putc_ret = gzputc(gzf, (int)(first_crc & 0xFF));
    int gz_flush_ret = gzflush(gzf, 0);
    int gz_close_ret = gzclose(gzf);
    free(comp_buf);
    (void)version;
    (void)source_len;
    (void)bound;
    (void)comp_size;
    (void)gz_putc_ret;
    (void)gz_flush_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}