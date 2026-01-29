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
//<ID> 1340
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source and deflate stream)
    const Bytef src[] = "Zlib API sequence test payload";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure (provide input and output buffers for deflate)
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);

    // step 3: Operate (compress with deflate, write original data to a gz file, then read it back)
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;
    int def_end_ret = deflateEnd(&strm);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_wrote = gzwrite(gw, src, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gw_close_w = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    off_t seek_pos = gzseek(gr, (off_t)0, 0);
    unsigned int read_len = (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len);
    Bytef *read_buf = (Bytef *)malloc((size_t)read_len + 1);
    memset(read_buf, 0, (int)(read_len + 1));
    int gr_read = gzread(gr, read_buf, read_len);
    int gr_eof = gzeof(gr);
    int gr_close_r = gzclose(gr);

    // step 4: Validate & Cleanup
    uLong read_crc = crc32(0L, read_buf, (uInt)(gr_read < 0 ? 0 : gr_read));
    free(comp_buf);
    free(read_buf);

    (void)init_ret;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)gw_wrote;
    (void)gw_close_w;
    (void)seek_pos;
    (void)gr_read;
    (void)gr_eof;
    (void)gr_close_r;
    (void)read_crc;
    (void)version;
    // API sequence test completed successfully
    return 66;
}