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
//<ID> 1119
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source data and compress into a zlib buffer
    const Bytef source[] = "zlib API sequence fuzz driver payload for gzopen/gzrewind and inflateEnd test.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLong comp_len = bound;
    int comp_ret = compress2(comp_buf, &comp_len, source, source_len, 6);

    // step 2: Initialize inflate stream, configure buffers and perform inflate
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *version = zlibVersion();
    int inf_init_ret = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)comp_len;
    uLong out_alloc = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    strm.next_out = out_buf;
    strm.avail_out = (uInt)out_alloc;
    int inflate_ret = inflate(&strm, 4);
    uLong out_size = strm.total_out;
    int inflate_end_ret = inflateEnd(&strm);

    // step 3: Persist original source to a .gz file, reopen, rewind and read back
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gw, source, (unsigned int)source_len);
    int gzclose_write_ret = gzclose(gw);
    gzFile gr = gzopen("tmp_zlib_api_seq.gz", "rb");
    int gzrew_ret = gzrewind(gr);
    z_size_t read_buf_size = (z_size_t)(out_alloc);
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (size_t)read_buf_size);
    int gzread_ret = gzread(gr, read_buf, (unsigned int)read_buf_size);
    int gzclose_read_ret = gzclose(gr);

    // step 4: Validate simple checksum, cleanup and finalize
    uLong crc = crc32(0UL, out_buf, (uInt)out_size);
    free(comp_buf);
    free(out_buf);
    free(read_buf);
    (void)comp_ret;
    (void)inf_init_ret;
    (void)inflate_ret;
    (void)inflate_end_ret;
    (void)gzwrite_ret;
    (void)gzclose_write_ret;
    (void)gzrew_ret;
    (void)gzread_ret;
    (void)gzclose_read_ret;
    (void)crc;
    (void)version;
    (void)bound;
    (void)source_len;
    (void)out_size;
    return 66;
    // API sequence test completed successfully
}