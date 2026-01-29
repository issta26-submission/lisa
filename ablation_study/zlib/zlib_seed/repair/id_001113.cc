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
//<ID> 1113
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and compress source to an in-memory buffer
    const Bytef source[] = "zlib API sequence payload for gzopen/gzclose/gzrewind and inflateEnd test.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, source, source_len, 6);

    // step 2: Initialize inflate stream, inflate the compressed buffer, then call inflateEnd
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
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

    // step 3: Write original source to a gzip file, close it, reopen, rewind and read back
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gw, source, (unsigned int)source_len);
    int gzclose_write_ret = gzclose(gw);
    gzFile gr = gzopen("tmp_zlib_api_seq.gz", "rb");
    int gzrewind_ret = gzrewind(gr);
    uLong read_alloc = source_len + 64;
    void *read_buf = malloc((size_t)read_alloc);
    memset(read_buf, 0, (size_t)read_alloc);
    int gzread_ret = gzread(gr, read_buf, (unsigned int)read_alloc);
    int gzclose_read_ret = gzclose(gr);

    // step 4: Validate checksum, cleanup and finalize
    uLong crc = crc32(0UL, out_buf, (uInt)out_size);
    free(comp_buf);
    free(out_buf);
    free(read_buf);
    (void)version;
    (void)bound;
    (void)comp_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)out_size;
    (void)gzwrite_ret;
    (void)gzclose_write_ret;
    (void)gzrewind_ret;
    (void)gzread_ret;
    (void)gzclose_read_ret;
    (void)crc;
    return 66;
    // API sequence test completed successfully
}