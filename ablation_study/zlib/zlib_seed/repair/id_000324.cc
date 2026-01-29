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
//<ID> 324
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver;
    z_stream def_src;
    z_stream def_copy;
    z_stream inf_strm;
    Bytef input_buf[] = "Example payload for zlib deflate/inflate with dictionary and gz I/O.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    uInt input_len_u = (uInt)input_len;
    Bytef compbuf[32768];
    Bytef decompbuf[32768];
    uLong comp_len;
    uLong decomp_len;
    int ret_def_src_init;
    int ret_def_call;
    int ret_def_copy;
    int ret_def_src_end;
    int ret_def_copy_end;
    int ret_inf_init;
    int ret_inf_setdict;
    int ret_inf_call;
    int ret_inf_end;
    gzFile gz;
    int gzwrite_ret;
    int gzflush_ret;

    // step 2: Setup / initialization
    ver = zlibVersion();
    memset(&def_src, 0, sizeof(def_src));
    memset(&def_copy, 0, sizeof(def_copy));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(decompbuf, 0, sizeof(decompbuf));
    ret_def_src_init = deflateInit_(&def_src, 6, ver, (int)sizeof(z_stream));
    def_src.next_in = (Bytef *)input_buf;
    def_src.avail_in = input_len_u;
    def_src.next_out = compbuf;
    def_src.avail_out = (uInt)sizeof(compbuf);

    // step 3: Core operations - compress, copy deflate state, inflate with dictionary, write gz
    ret_def_call = deflate(&def_src, 4);
    comp_len = def_src.total_out;
    ret_def_copy = deflateCopy(&def_copy, &def_src);
    ret_def_copy_end = deflateEnd(&def_copy);
    ret_inf_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decompbuf;
    inf_strm.avail_out = (uInt)sizeof(decompbuf);
    ret_inf_setdict = inflateSetDictionary(&inf_strm, (const Bytef *)input_buf, input_len_u);
    ret_inf_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    gz = gzopen("test_zlib_api_sequence.gz", "wb+");
    gzwrite_ret = gzwrite(gz, (voidpc)compbuf, (unsigned int)comp_len);
    gzflush_ret = gzflush(gz, 2);
    gzclearerr(gz);

    // step 4: Cleanup / validation
    ret_def_src_end = deflateEnd(&def_src);
    ret_inf_end = inflateEnd(&inf_strm);
    gzclose(gz);
    (void)ver;
    (void)input_len;
    (void)input_len_u;
    (void)comp_len;
    (void)decomp_len;
    (void)ret_def_src_init;
    (void)ret_def_call;
    (void)ret_def_copy;
    (void)ret_def_src_end;
    (void)ret_def_copy_end;
    (void)ret_inf_init;
    (void)ret_inf_setdict;
    (void)ret_inf_call;
    (void)ret_inf_end;
    (void)gzwrite_ret;
    (void)gzflush_ret;
    // API sequence test completed successfully
    return 66;
}