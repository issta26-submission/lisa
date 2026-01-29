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
//<ID> 1285
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence payload: compress, decompress, write to gz, seek and close using gzclose_w/gzclose_r";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    uLong max_comp_bound = deflateBound(&defstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)max_comp_bound);
    memset(comp_buf, 0, (int)max_comp_bound);
    defstrm.next_in = (Bytef *)src_data;
    defstrm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)(max_comp_bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)max_comp_bound);

    // step 2: Operate - deflate the source
    int def_ret = deflate(&defstrm, 4);
    uLong comp_size = defstrm.total_out;
    int def_end_ret = deflateEnd(&defstrm);

    // step 3: Configure & Operate - inflate the produced compressed buffer and inspect codes used
    z_stream instrm;
    memset(&instrm, 0, (int)sizeof(instrm));
    int inf_init_ret = inflateInit_(&instrm, version, (int)sizeof(z_stream));
    Bytef *decomp_buf = (Bytef *)malloc((size_t)src_len + 1);
    memset(decomp_buf, 0, (int)(src_len + 1));
    instrm.next_in = comp_buf;
    instrm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    instrm.next_out = decomp_buf;
    instrm.avail_out = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    int inf_ret = inflate(&instrm, 4);
    unsigned long codes_used = inflateCodesUsed(&instrm);
    int inf_end_ret = inflateEnd(&instrm);

    // step 4: Validate & Cleanup - write compressed data to a gz file, seek, and close with gzclose_w/gzclose_r
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gzw, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    off_t gz_pos_after_write = gzseek(gzw, 0, 1);
    int gzclosew_ret = gzclose_w(gzw);
    gzFile gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    off_t gz_pos_read = gzseek(gzr, 0, 0);
    int gzcloser_ret = gzclose_r(gzr);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)def_init_ret;
    (void)max_comp_bound;
    (void)def_ret;
    (void)comp_size;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)inf_end_ret;
    (void)gz_write_ret;
    (void)gz_pos_after_write;
    (void)gzclosew_ret;
    (void)gz_pos_read;
    (void)gzcloser_ret;
    // API sequence test completed successfully
    return 66;
}