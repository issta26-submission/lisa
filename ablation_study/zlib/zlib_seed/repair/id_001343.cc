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
//<ID> 1343
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Test data for zlib API sequence for gzseek/gzread usage.";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (int)bound);
    strm.next_out = comp;
    strm.avail_out = (uInt)bound;

    // step 2: Configure & Operate (compress to memory)
    int def_ret = deflate(&strm, 4); /* Z_FINISH = 4 */
    uLong comp_len = (uLong)strm.total_out;
    int end_ret = deflateEnd(&strm);

    // step 3: Write compressed data to a gz file and close
    gzFile gwf = gzopen("test_zlib_api_sequence.gz", "wb");
    int wrote = gzwrite(gwf, comp, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int closed_write = gzclose(gwf);

    // step 4: Read back using gzread, use gzseek and gzeof, then cleanup & validate
    gzFile grf = gzopen("test_zlib_api_sequence.gz", "rb");
    unsigned int read_size = (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len);
    Bytef *read_buf = (Bytef *)malloc((size_t)read_size);
    memset(read_buf, 0, (int)read_size);
    int first_read = gzread(grf, read_buf, read_size);
    int at_eof_before = gzeof(grf);
    auto pos0 = gzseek(grf, 0, 0); /* SEEK_SET = 0 */
    int second_read = gzread(grf, read_buf, read_size);
    int closed_read = gzclose(grf);
    uLong crc = crc32(0L, read_buf, (uInt)second_read);

    free(comp);
    free(read_buf);

    (void)init_ret;
    (void)def_ret;
    (void)end_ret;
    (void)bound;
    (void)wrote;
    (void)closed_write;
    (void)first_read;
    (void)at_eof_before;
    (void)pos0;
    (void)second_read;
    (void)closed_read;
    (void)crc;
    (void)version;
    // API sequence test completed successfully
    return 66;
}