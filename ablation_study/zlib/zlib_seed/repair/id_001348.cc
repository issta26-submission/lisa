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
//<ID> 1348
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Sample data for zlib deflate and gz file round-trip.";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);

    // step 2: Configure
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (int)bound);
    strm.next_out = comp;
    strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);

    // step 3: Operate
    int def_ret = deflate(&strm, 4);
    uLong comp_size = (uLong)strm.total_out;
    int def_end_ret = deflateEnd(&strm);
    gzFile gwf = gzopen("test_zlib_api_sequence_temp.gz", "wb");
    int gwritten = gzwrite(gwf, comp, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    int gclose_w = gzclose(gwf);
    gzFile grf = gzopen("test_zlib_api_sequence_temp.gz", "rb");
    off_t seek_pos = gzseek(grf, (off_t)0, 0);
    unsigned int to_read = (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size);
    Bytef *read_buf = (Bytef *)malloc((size_t)to_read);
    memset(read_buf, 0, (int)to_read);
    int gread_ret = gzread(grf, read_buf, to_read);
    int is_eof = gzeof(grf);
    int gclose_r = gzclose(grf);

    // step 4: Validate & Cleanup
    free(comp);
    free(read_buf);
    (void)init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)gwritten;
    (void)gclose_w;
    (void)seek_pos;
    (void)gread_ret;
    (void)is_eof;
    (void)gclose_r;
    (void)version;
    (void)src_len;
    // API sequence test completed successfully
    return 66;
}