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
//<ID> 1057
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence test payload: original data to write to gz, and also compress via deflate for API coverage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure / Operate (deflate twice with a reset between)
    uLong bound = deflateBound(&dstrm, source_len);
    Bytef *comp_buf1 = (Bytef *)malloc((size_t)bound);
    memset(comp_buf1, 0, (size_t)bound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)source_len;
    dstrm.next_out = comp_buf1;
    dstrm.avail_out = (uInt)bound;
    int def_ret1 = deflate(&dstrm, 4);
    uLong comp_size1 = dstrm.total_out;
    int reset_ret = deflateReset(&dstrm);
    Bytef *comp_buf2 = (Bytef *)malloc((size_t)bound);
    memset(comp_buf2, 0, (size_t)bound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)source_len;
    dstrm.next_out = comp_buf2;
    dstrm.avail_out = (uInt)bound;
    int def_ret2 = deflate(&dstrm, 4);
    uLong comp_size2 = dstrm.total_out;

    // step 3: Operate with gz I/O and read back using gzfread, query offset, then close
    gzFile gzw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzw, source, (unsigned int)source_len);
    int gzclose_ret_w = gzclose(gzw);
    gzFile gzr = gzopen("tmp_zlib_api_seq.gz", "rb");
    z_size_t read_buf_len = (z_size_t)(source_len + 64);
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_len);
    memset(read_buf, 0, (size_t)read_buf_len);
    z_size_t read_items = gzfread(read_buf, 1, read_buf_len, gzr);
    off_t current_offset = gzoffset(gzr);
    int gzclose_ret_r = gzclose(gzr);

    // step 4: Validate / Cleanup
    uLong checksum = adler32_z(0UL, read_buf, read_items);
    int def_end_ret = deflateEnd(&dstrm);
    free(comp_buf1);
    free(comp_buf2);
    free(read_buf);
    (void)version;
    (void)def_init_ret;
    (void)def_ret1;
    (void)comp_size1;
    (void)reset_ret;
    (void)def_ret2;
    (void)comp_size2;
    (void)gzwrite_ret;
    (void)gzclose_ret_w;
    (void)read_items;
    (void)current_offset;
    (void)gzclose_ret_r;
    (void)checksum;
    (void)bound;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}