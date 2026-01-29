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
//<ID> 1091
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / Configure
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence payload for compress2, inflate, gz operations and deflateEnd.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong comp_bound = compressBound(source_len);
    uLongf comp_len = (uLongf)comp_bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_len);
    memset(comp_buf, 0, (size_t)comp_len);
    int comp_ret = compress2(comp_buf, &comp_len, source, source_len, 6);

    // step 2: Write compressed bytes to a .gz file
    gzFile gzw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzw, (voidpc)comp_buf, (unsigned int)comp_len);
    int gzclosew_ret = gzclose(gzw);

    // step 3: Initialize inflate and perform a single inflate call; also prepare a deflate stream to call deflateEnd later
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int init_in_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len;
    uLong out_size = source_len + 64;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_size);
    memset(out_buf, 0, (size_t)out_size);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)out_size;
    int infl_ret = inflate(&istrm, 0);

    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int init_def_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 4: Open the gz file for reading, read one byte via gzgetc_, close with gzclose_r, finalize deflate stream and cleanup
    gzFile gzr = gzopen("tmp_zlib_api_seq.gz", "rb");
    int ch = gzgetc_(gzr);
    int gzcloser_ret = gzclose_r(gzr);
    int defend_ret = deflateEnd(&dstrm);
    free(comp_buf);
    free(out_buf);
    (void)comp_ret;
    (void)comp_len;
    (void)gzwrite_ret;
    (void)gzclosew_ret;
    (void)init_in_ret;
    (void)infl_ret;
    (void)init_def_ret;
    (void)ch;
    (void)gzcloser_ret;
    (void)defend_ret;
    (void)version;
    (void)source_len;
    // API sequence test completed successfully
    return 66;
}