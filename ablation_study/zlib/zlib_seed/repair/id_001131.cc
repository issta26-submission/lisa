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
//<ID> 1131
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source and deflate stream
    const Bytef source[] = "zlib API sequence fuzz driver payload for deflate/inflate and gzwrite/gzread test.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;

    // step 2: Persist original source to a .gz file using gzwrite and close
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gw, (voidpc)source, (unsigned int)source_len);
    int gzclose_write_ret = gzclose(gw);

    // step 3: Initialize inflate stream and decompress the deflated buffer
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    uLong out_alloc = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_size;
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)out_alloc;
    int inf_ret = inflate(&inf_strm, 4);
    uLong out_size = inf_strm.total_out;

    // step 4: Validate with crc32, read back gz file, finalize and cleanup
    uLong crc = crc32(0UL, out_buf, (uInt)out_size);
    gzFile gr = gzopen("tmp_zlib_api_seq.gz", "rb");
    z_size_t read_buf_size = (z_size_t)(out_alloc);
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (size_t)read_buf_size);
    int gzread_ret = gzread(gr, read_buf, (unsigned int)read_buf_size);
    int gzclose_read_ret = gzclose(gr);
    int inf_end_ret = inflateEnd(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    free(out_buf);
    free(read_buf);
    (void)def_init_ret;
    (void)def_ret;
    (void)comp_size;
    (void)gzwrite_ret;
    (void)gzclose_write_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)out_size;
    (void)crc;
    (void)gzread_ret;
    (void)gzclose_read_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    (void)version;
    (void)bound;
    (void)read_buf_size;
    return 66; // API sequence test completed successfully
}