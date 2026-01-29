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
//<ID> 1052
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API fuzz driver: primary payload to compress and later validate via gz I/O.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure / Operate (first compression, then reset and compress a small header)
    uLong bound1 = deflateBound(&dstrm, source_len);
    Bytef *comp1 = (Bytef *)malloc((size_t)bound1);
    memset(comp1, 0, (size_t)bound1);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)source_len;
    dstrm.next_out = comp1;
    dstrm.avail_out = (uInt)bound1;
    int def_ret1 = deflate(&dstrm, 4);
    uLong comp1_size = dstrm.total_out;
    int reset_ret = deflateReset(&dstrm);
    const Bytef header[] = "hdr:meta";
    uLong header_len = (uLong)(sizeof(header) - 1);
    uLong bound2 = deflateBound(&dstrm, header_len);
    Bytef *comp2 = (Bytef *)malloc((size_t)bound2);
    memset(comp2, 0, (size_t)bound2);
    dstrm.next_in = (Bytef *)header;
    dstrm.avail_in = (uInt)header_len;
    dstrm.next_out = comp2;
    dstrm.avail_out = (uInt)bound2;
    int def_ret2 = deflate(&dstrm, 4);
    uLong comp2_size = dstrm.total_out;
    int def_end_ret = deflateEnd(&dstrm);

    // step 3: Write compressed data to a gz file, inspect offset, then read it back with gzfread and inspect offset
    gzFile gzw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzw, comp1, (unsigned int)comp1_size);
    int gzwrite_ret2 = gzwrite(gzw, comp2, (unsigned int)comp2_size);
    off_t write_offset = gzoffset(gzw);
    int gzclose_ret_w = gzclose(gzw);
    gzFile gzr = gzopen("tmp_zlib_api_seq.gz", "rb");
    z_size_t read_buf_len = (z_size_t)(comp1_size + comp2_size + 64);
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_len);
    memset(read_buf, 0, (size_t)read_buf_len);
    z_size_t read_items = gzfread(read_buf, (z_size_t)1, read_buf_len, gzr);
    off_t read_offset = gzoffset(gzr);
    int gzclose_ret_r = gzclose(gzr);

    // step 4: Validate / Cleanup
    uLong checksum = adler32_z(0UL, read_buf, (z_size_t)read_items);
    free(comp1);
    free(comp2);
    free(read_buf);
    (void)init_ret;
    (void)def_ret1;
    (void)comp1_size;
    (void)reset_ret;
    (void)def_ret2;
    (void)comp2_size;
    (void)def_end_ret;
    (void)gzwrite_ret;
    (void)gzwrite_ret2;
    (void)write_offset;
    (void)gzclose_ret_w;
    (void)read_items;
    (void)read_offset;
    (void)gzclose_ret_r;
    (void)checksum;
    (void)version;
    (void)source_len;
    (void)bound1;
    (void)bound2;
    // API sequence test completed successfully
    return 66;
}