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
//<ID> 1136
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and initialize deflate stream
    const Bytef source[] = "zlib API sequence fuzz driver payload for deflate/inflate and gzwrite/gzclose test.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)source_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)bound;

    // step 2: Perform deflate and finalize deflate, then initialize and perform inflate
    int def_ret = deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    int def_end_ret = deflateEnd(&dstrm);
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_size;
    uLong out_alloc = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)out_alloc;
    int inflate_ret = inflate(&istrm, 4);
    uLong out_size = istrm.total_out;
    int inflate_end_ret = inflateEnd(&istrm);

    // step 3: Persist original source using gzwrite/gzclose and then read it back
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gw, source, (unsigned int)source_len);
    int gzclose_write_ret = gzclose(gw);
    gzFile gr = gzopen("tmp_zlib_api_seq.gz", "rb");
    z_size_t read_buf_size = (z_size_t)(source_len + 128);
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (size_t)read_buf_size);
    int gzread_ret = gzread(gr, read_buf, (unsigned int)read_buf_size);
    int gzclose_read_ret = gzclose(gr);

    // step 4: Validate with crc32, cleanup resources and finalize
    uLong crc = crc32(0UL, out_buf, (uInt)out_size);
    free(comp_buf);
    free(out_buf);
    free(read_buf);
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_size;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inflate_ret;
    (void)inflate_end_ret;
    (void)version;
    (void)gw;
    (void)gzwrite_ret;
    (void)gzclose_write_ret;
    (void)gr;
    (void)gzread_ret;
    (void)gzclose_read_ret;
    (void)crc;
    (void)out_size;
    (void)read_buf_size;
    return 66;
    // API sequence test completed successfully
}