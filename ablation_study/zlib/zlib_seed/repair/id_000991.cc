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
//<ID> 991
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence: write original to gz, deflate into buffer, inspect pending bits and crc table, then cleanup.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 0);
    unsigned int pending_bytes = 0;
    int pending_bits = 0;
    int pending_ret = deflatePending(&def_strm, &pending_bytes, &pending_bits);
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t first_crc = crc_table[0];
    gzFile gzf = gzdopen(1, "wb");
    int gz_puts_ret = gzputs(gzf, (const char *)source);
    int gz_close_ret = gzclose(gzf);

    // step 4: Validate / Cleanup
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)pending_ret;
    (void)pending_bytes;
    (void)pending_bits;
    (void)first_crc;
    (void)gz_puts_ret;
    (void)gz_close_ret;
    (void)comp_size;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}