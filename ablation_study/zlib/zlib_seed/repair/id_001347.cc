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
//<ID> 1347
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and version)
    const Bytef src_data[] = "Example data for zlib: compress, write to gz, seek, read back";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();

    // step 2: Configure (initialize deflate stream and output buffer)
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src_data;
    def_strm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 3: Operate (compress, write to gz, close, reopen, seek, read)
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = (uLong)def_strm.total_out;
    int end_ret = deflateEnd(&def_strm);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gwritten = gzwrite(gw, comp_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int gclose_w = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    off_t seek_ret = gzseek(gr, (off_t)0, 0);
    Bytef *read_buf = (Bytef *)malloc((size_t)(comp_len + 16));
    memset(read_buf, 0, (int)(comp_len + 16));
    int gread_bytes = gzread(gr, read_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int is_eof = gzeof(gr);
    int gclose_r = gzclose(gr);

    // step 4: Validate & Cleanup (compute simple CRCs, free buffers)
    uLong crc_src = crc32(0L, src_data, (uInt)src_len);
    uLong crc_comp = crc32(0L, read_buf, (uInt)(gread_bytes > 0 ? (uInt)gread_bytes : 0U));
    uLong combined = crc_src ^ crc_comp;
    free(comp_buf);
    free(read_buf);

    (void)init_ret;
    (void)def_ret;
    (void)end_ret;
    (void)gwritten;
    (void)gclose_w;
    (void)seek_ret;
    (void)gread_bytes;
    (void)is_eof;
    (void)gclose_r;
    (void)crc_src;
    (void)crc_comp;
    (void)combined;
    (void)version;
    // API sequence test completed successfully
    return 66;
}