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
//<ID> 1096
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / Configure
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence fuzz driver payload to deflate, write gz, read gz, and inflate.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    memset(&istrm, 0, (int)sizeof(istrm));
    int init_def_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong comp_bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);

    // step 2: Operate (compress with deflate)
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)source_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)comp_bound;
    int def_ret = deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;

    // step 3: Persist compressed bytes to .gz, then read one byte via gzgetc_ and close with gzclose_r; also inflate in-memory
    gzFile gzw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzw, (voidpc)comp_buf, (unsigned int)comp_size);
    int gzclose_w_ret = gzclose(gzw);
    gzFile gzr = gzopen("tmp_zlib_api_seq.gz", "rb");
    int first_byte = gzgetc_(gzr);
    int gzclose_r_ret = gzclose_r(gzr);

    int init_inf_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong out_bound = source_len + 16;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (int)out_bound);
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)out_bound;
    int inf_ret = inflate(&istrm, 0);
    uLong decompressed = istrm.total_out;

    // step 4: Cleanup / Validate / Finalize
    int end_def_ret = deflateEnd(&dstrm);
    int end_inf_ret = inflateEnd(&istrm);
    free(comp_buf);
    free(out_buf);
    (void)init_def_ret;
    (void)comp_bound;
    (void)def_ret;
    (void)comp_size;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)first_byte;
    (void)gzclose_r_ret;
    (void)init_inf_ret;
    (void)inf_ret;
    (void)decompressed;
    (void)end_def_ret;
    (void)end_inf_ret;
    (void)version;
    (void)source_len;
    // API sequence test completed successfully
    return 66;
}