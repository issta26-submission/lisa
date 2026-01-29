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
//<ID> 1090
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into an in-memory buffer
    const Bytef source[] = "zlib API sequence test payload for compress2, write to .gz, read back, inflate and deflate cleanup.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLongf comp_cap = (uLongf)(source_len + 256);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_cap);
    memset(comp_buf, 0, (size_t)comp_cap);
    uLongf comp_len = comp_cap;
    int comp_ret = compress2(comp_buf, &comp_len, source, source_len, 6);

    // step 2: Write compressed data to a .gz file, close, reopen for reading, read one byte, and close read handle
    gzFile gzf_w = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzf_w, (voidpc)comp_buf, (unsigned int)comp_len);
    int gzclose_w_ret = gzclose(gzf_w);
    gzFile gzf_r = gzopen("tmp_zlib_api_seq.gz", "rb");
    int first_char = gzgetc_(gzf_r);
    int gzclose_r_ret = gzclose_r(gzf_r);

    // step 3: Initialize inflate stream, decompress the in-memory compressed buffer in one call
    z_stream instrm;
    memset(&instrm, 0, (int)sizeof(instrm));
    const char *version = zlibVersion();
    int inf_init_ret = inflateInit_(&instrm, version, (int)sizeof(z_stream));
    Bytef *out_buf = (Bytef *)malloc((size_t)source_len);
    memset(out_buf, 0, (size_t)source_len);
    instrm.next_in = comp_buf;
    instrm.avail_in = (uInt)comp_len;
    instrm.next_out = out_buf;
    instrm.avail_out = (uInt)source_len;
    int inflate_ret = inflate(&instrm, 0);
    uLong decompressed = instrm.total_out;

    // step 4: Initialize a deflate stream and immediately end it to exercise deflateEnd, then cleanup
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int def_end_ret = deflateEnd(&dstrm);
    free(comp_buf);
    free(out_buf);
    (void)comp_ret;
    (void)comp_len;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)first_char;
    (void)gzclose_r_ret;
    (void)inf_init_ret;
    (void)inflate_ret;
    (void)decompressed;
    (void)def_init_ret;
    (void)def_end_ret;
    (void)version;
    (void)source_len;
    // API sequence test completed successfully
    return 66;
}