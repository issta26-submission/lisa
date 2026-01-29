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
//<ID> 1063
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / Prepare data and compression buffers
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence payload for compress->inflate->gzwrite->crc combine demonstration.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len_f = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len_f, source, source_len, 6);
    uLong comp_size = (uLong)comp_len_f;

    // step 2: Configure inflate stream
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = inflateInit_(&strm, version, (int)sizeof(z_stream));
    Bytef *out_buf = (Bytef *)malloc((size_t)(source_len + 16));
    memset(out_buf, 0, (size_t)(source_len + 16));
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)comp_size;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)(source_len + 16);
    int undermine_ret = inflateUndermine(&strm, 1);

    // step 3: Operate (inflate compressed data)
    int inflate_ret = inflate(&strm, 4);
    uLong decomp_size = strm.total_out;
    int inflate_end_ret = inflateEnd(&strm);

    // step 4: Write compressed data to gz, seek, compute and combine CRCs, cleanup
    gzFile gzf = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_size);
    off_t seek_pos = gzseek(gzf, (off_t)0, 0);
    int gzclose_ret = gzclose(gzf);
    uLong crc_orig = crc32(0UL, source, (uInt)source_len);
    uLong crc_decomp = crc32(0UL, out_buf, (uInt)decomp_size);
    uLong crc_combined = crc32_combine(crc_orig, crc_decomp, (off_t)decomp_size);
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)comp_ret;
    (void)init_ret;
    (void)undermine_ret;
    (void)inflate_ret;
    (void)inflate_end_ret;
    (void)gzwrite_ret;
    (void)seek_pos;
    (void)gzclose_ret;
    (void)crc_orig;
    (void)crc_decomp;
    (void)crc_combined;
    // API sequence test completed successfully
    return 66;
}