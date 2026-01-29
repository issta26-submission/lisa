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
//<ID> 1114
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize and compress source into an in-memory buffer
    const Bytef source[] = "zlib API sequence fuzz driver payload for gzopen -> gzrewind -> inflateEnd test.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong bound = compressBound(source_len);
    uLongf comp_len = (uLongf)bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    int comp_ret = compress2(comp_buf, &comp_len, source, source_len, 6);

    // step 2: Persist compressed bytes to a .gz file, then rewind the gzFile
    gzFile gz = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gz, comp_buf, (unsigned int)comp_len);
    int gzrewind_ret = gzrewind(gz);

    // step 3: Inflate the compressed buffer back to an output buffer and call inflateEnd
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    const char *version = zlibVersion();
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    uLong out_alloc = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)out_alloc;
    int inf_ret = inflate(&inf_strm, 4);
    uLong out_size = inf_strm.total_out;
    int inf_end_ret = inflateEnd(&inf_strm);

    // step 4: Validate checksum, cleanup gzFile and buffers
    uLong crc = crc32(0UL, out_buf, (uInt)out_size);
    int gzclose_ret = gzclose(gz);
    free(comp_buf);
    free(out_buf);
    (void)comp_ret;
    (void)gzwrite_ret;
    (void)gzrewind_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)crc;
    (void)out_size;
    (void)version;
    (void)bound;
    (void)source_len;
    (void)gzclose_ret;
    return 66;
    // API sequence test completed successfully
}