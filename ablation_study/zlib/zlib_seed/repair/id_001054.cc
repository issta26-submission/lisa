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
//<ID> 1054
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence: compress to buffer, write to gz, read back with gzfread, inspect offset and cleanup.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int reset_ret = deflateReset(&dstrm);

    // step 2: Configure and operate deflate
    uLong bound = deflateBound(&dstrm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)source_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)bound;
    int def_ret = deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    int def_end_ret = deflateEnd(&dstrm);

    // step 3: Write compressed data to a gz file and read it back using gzfread, inspect offsets
    gzFile gzw = gzopen("tmp_zlib_api_sequence.gz", "wb");
    int gzwrite_ret = gzwrite(gzw, comp_buf, (unsigned int)comp_size);
    off_t write_offset = gzoffset(gzw);
    int gzclose_w = gzclose(gzw);
    gzFile gzr = gzopen("tmp_zlib_api_sequence.gz", "rb");
    z_size_t read_buf_len = (z_size_t)(source_len + 128);
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_len);
    memset(read_buf, 0, (size_t)read_buf_len);
    z_size_t read_items = gzfread(read_buf, 1, read_buf_len, gzr);
    off_t read_offset = gzoffset(gzr);
    int gzclose_r = gzclose(gzr);

    // step 4: Validate / Cleanup
    uLong checksum = adler32_z(0UL, read_buf, read_items);
    free(comp_buf);
    free(read_buf);
    (void)init_ret;
    (void)reset_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)gzwrite_ret;
    (void)write_offset;
    (void)read_items;
    (void)read_offset;
    (void)gzclose_w;
    (void)gzclose_r;
    (void)checksum;
    (void)version;
    (void)source_len;
    (void)bound;
    (void)comp_size;
    // API sequence test completed successfully
    return 66;
}