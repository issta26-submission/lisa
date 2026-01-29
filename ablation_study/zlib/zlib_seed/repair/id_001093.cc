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
//<ID> 1093
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / Configure deflate stream and prepare source buffer
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence test payload: compress to .gz, read one byte, then inflate back.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int definit_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 2: Operate - perform deflate and finalize deflate stream
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Write compressed bytes to a .gz file, reopen for reading, read one byte and close read handle
    gzFile gz_w = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gz_w, (voidpc)comp_buf, (unsigned int)comp_size);
    int gzclose_w_ret = gzclose(gz_w);
    gzFile gz_r = gzopen("tmp_zlib_api_seq.gz", "rb");
    int first_byte = gzgetc_(gz_r);
    int gzclose_r_ret = gzclose_r(gz_r);

    // step 4: Inflate the in-memory compressed buffer to verify decompression and cleanup
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int infinit_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    Bytef *out_buf = (Bytef *)malloc((size_t)(source_len + 32));
    memset(out_buf, 0, (size_t)(source_len + 32));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_size;
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)(source_len + 32);
    int inf_ret = inflate(&inf_strm, 0);
    int inf_end_ret = inflateEnd(&inf_strm);

    uLong crc = crc32(0UL, out_buf, (uInt)inf_strm.total_out);

    free(comp_buf);
    free(out_buf);

    (void)version;
    (void)definit_ret;
    (void)bound;
    (void)def_ret;
    (void)def_end_ret;
    (void)comp_size;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)first_byte;
    (void)gzclose_r_ret;
    (void)infinit_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)crc;
    // API sequence test completed successfully
    return 66;
}