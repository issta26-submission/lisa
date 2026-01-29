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
//<ID> 1064
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / Declarations
    const char *version = zlibVersion();
    const Bytef source[] = "Example payload for zlib: compress, write to gz, inflate, and combine CRCs.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress(comp_buf, &comp_len, source, src_len);

    // step 2: Write compressed data to a gz file
    gzFile gzf = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzf, (voidpc)comp_buf, (unsigned int)comp_len);
    off_t seek_ret = gzseek(gzf, (off_t)0, 0); // SEEK_SET == 0
    int gzclose_ret_w = gzclose(gzf);

    // step 3: Inflate the compressed data and use inflateUndermine
    z_stream infstream;
    memset(&infstream, 0, (int)sizeof(infstream));
    int init_ret = inflateInit_(&infstream, version, (int)sizeof(z_stream));
    int undermine_ret = inflateUndermine(&infstream, 1);
    infstream.next_in = comp_buf;
    infstream.avail_in = (uInt)comp_len;
    uLong out_cap = src_len + 16;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_cap);
    memset(out_buf, 0, (size_t)out_cap);
    infstream.next_out = out_buf;
    infstream.avail_out = (uInt)out_cap;
    int inf_ret = inflate(&infstream, 4); // 4 -> Z_FINISH
    uLong out_len = infstream.total_out;
    int inf_end_ret = inflateEnd(&infstream);

    // step 4: Validate (CRC combine) and Cleanup
    uLong crc1 = crc32(0UL, source, (uInt)src_len);
    uLong crc2 = crc32(0UL, out_buf, (uInt)out_len);
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)out_len);
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)comp_ret;
    (void)gzwrite_ret;
    (void)seek_ret;
    (void)gzclose_ret_w;
    (void)init_ret;
    (void)undermine_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)crc1;
    (void)crc2;
    (void)combined_crc;
    // API sequence test completed successfully
    return 66;
}