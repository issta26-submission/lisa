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
//<ID> 785
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef input[256];
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
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;

    // step 2: Operate with gzip I/O
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile wgz = gzopen64(fname, "wb");
    int gz_wr = gzwrite(wgz, comp, (unsigned int)comp_len);
    int gz_close_w = gzclose_r(wgz);
    gzFile rgz = gzopen64(fname, "rb");
    int first_byte = gzgetc_(rgz);

    // step 3: Inflate initialization and copy
    z_stream inf_src;
    memset(&inf_src, 0, sizeof(inf_src));
    inflateInit_(&inf_src, version, (int)sizeof(z_stream));
    inf_src.next_in = comp;
    inf_src.avail_in = (uInt)comp_len;
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    int copy_ret = inflateCopy(&inf_copy, &inf_src);
    int inf_copy_end = inflateEnd(&inf_copy);
    int inf_src_end = inflateEnd(&inf_src);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def);
    int gz_close_r_ret = gzclose_r(rgz);
    free(comp);

    (void)version;
    (void)input_len;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)fname;
    (void)gz_wr;
    (void)gz_close_w;
    (void)first_byte;
    (void)copy_ret;
    (void)inf_copy_end;
    (void)inf_src_end;
    (void)def_end_ret;
    (void)gz_close_r_ret;
    // API sequence test completed successfully
    return 66;
}