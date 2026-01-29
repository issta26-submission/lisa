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
//<ID> 786
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef input[128];
    memset(input, 'A', sizeof(input));
    uLong input_len = (uLong)sizeof(input);

    z_stream def;
    memset(&def, 0, sizeof(def));
    deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = input;
    def.avail_in = (uInt)input_len;
    uLong bound = deflateBound(&def, input_len);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    def.next_out = comp;
    def.avail_out = (uInt)bound;

    // step 2: Configure / Operate (compress)
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;
    int def_end_ret = deflateEnd(&def);

    // step 3: Initialize inflate and clone state
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = comp;
    inf.avail_in = (uInt)comp_len;
    Bytef *out = (Bytef *)malloc((size_t)input_len);
    memset(out, 0, (size_t)input_len);
    inf.next_out = out;
    inf.avail_out = (uInt)input_len;
    int inf_ret = inflate(&inf, 0);
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    int copy_ret = inflateCopy(&inf_copy, &inf);

    // step 4: Write compressed data to a .gz file, read one byte back, cleanup
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gz_w = gzopen64(fname, "wb");
    int gz_wr = 0;
    if (gz_w) gz_wr = gzwrite(gz_w, comp, (unsigned int)comp_len);
    if (gz_w) gzclose(gz_w);
    gzFile gz_r = gzopen64(fname, "rb");
    int first_byte = -1;
    if (gz_r) first_byte = gzgetc_(gz_r);
    int gz_close_read_ret = gzclose_r(gz_r);

    int inf_end_ret = inflateEnd(&inf);
    int inf_copy_end_ret = inflateEnd(&inf_copy);
    free(comp);
    free(out);

    (void)version;
    (void)input_len;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)inf_ret;
    (void)copy_ret;
    (void)fname;
    (void)gz_wr;
    (void)first_byte;
    (void)gz_close_read_ret;
    (void)inf_end_ret;
    (void)inf_copy_end_ret;
    // API sequence test completed successfully
    return 66;
}