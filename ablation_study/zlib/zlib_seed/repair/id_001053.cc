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
//<ID> 1053
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence test payload: compress, write to gz, read back, validate checksum.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int def_reset_ret = deflateReset(&dstrm);

    // step 2: Configure
    uLong bound = deflateBound(&dstrm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)source_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    int def_end_ret = deflateEnd(&dstrm);

    // step 4: Validate / Cleanup
    gzFile gzw = gzopen("tmp_zlib_api_sequence.gz", "wb");
    int gzwrite_ret = gzwrite(gzw, comp_buf, (unsigned int)comp_size);
    int gzclose_ret_w = gzclose(gzw);
    gzFile gzr = gzopen("tmp_zlib_api_sequence.gz", "rb");
    z_size_t read_buf_len = (z_size_t)(source_len + 128);
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_len);
    memset(read_buf, 0, (size_t)read_buf_len);
    z_size_t read_items = gzfread(read_buf, 1, read_buf_len, gzr);
    off_t offset = gzoffset(gzr);
    int gzclose_ret_r = gzclose(gzr);
    uLong checksum = adler32_z(0UL, read_buf, read_items);
    free(comp_buf);
    free(read_buf);
    (void)version;
    (void)def_init_ret;
    (void)def_reset_ret;
    (void)bound;
    (void)def_ret;
    (void)def_end_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret_w;
    (void)gzclose_ret_r;
    (void)offset;
    (void)checksum;
    // API sequence test completed successfully
    return 66;
}