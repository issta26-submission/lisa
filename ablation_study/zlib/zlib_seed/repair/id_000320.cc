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
//<ID> 320
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver;
    z_stream src_def;
    z_stream dst_def;
    z_stream inf_strm;
    Bytef input_buf[] = "Example payload for zlib compression and dictionary tests.";
    uInt input_len = (uInt)(sizeof(input_buf) - 1);
    Bytef dict_buf[] = "example_dictionary";
    uInt dict_len = (uInt)(sizeof(dict_buf) - 1);
    Bytef compbuf[16384];
    Bytef decompbuf[16384];
    uLong final_comp_len;
    uLong decomp_len;
    gzFile gz;
    int ret_def_init_src;
    int ret_def_call_src;
    int ret_def_copy;
    int ret_def_call_dst;
    int ret_def_end_src;
    int ret_def_end_dst;
    int ret_inf_init;
    int ret_inf_setdict;
    int ret_inf_call;
    int ret_inf_end;
    int gzwrite_ret;
    int gzflush_ret;
    int gzclose_ret;

    // step 2: Setup / initialization
    ver = zlibVersion();
    memset(&src_def, 0, sizeof(src_def));
    memset(&dst_def, 0, sizeof(dst_def));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(decompbuf, 0, sizeof(decompbuf));
    ret_def_init_src = deflateInit_(&src_def, 6, ver, (int)sizeof(z_stream));
    src_def.next_in = input_buf;
    src_def.avail_in = input_len;
    src_def.next_out = compbuf;
    src_def.avail_out = (uInt)sizeof(compbuf);

    // step 3: Core operations - compress, copy deflate state, inflate with dictionary, gz operations
    ret_def_call_src = deflate(&src_def, 4);
    final_comp_len = src_def.total_out;
    ret_def_copy = deflateCopy(&dst_def, &src_def);
    dst_def.next_out = compbuf + (uInt)final_comp_len;
    dst_def.avail_out = (uInt)(sizeof(compbuf) - final_comp_len);
    ret_def_call_dst = deflate(&dst_def, 4);
    final_comp_len = dst_def.total_out;
    ret_def_end_src = deflateEnd(&src_def);
    ret_def_end_dst = deflateEnd(&dst_def);
    ret_inf_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)final_comp_len;
    inf_strm.next_out = decompbuf;
    inf_strm.avail_out = (uInt)sizeof(decompbuf);
    ret_inf_setdict = inflateSetDictionary(&inf_strm, dict_buf, dict_len);
    ret_inf_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inf_end = inflateEnd(&inf_strm);
    gz = gzopen("test_zlib_api_sequence.gz", "wb+");
    gzwrite_ret = gzwrite(gz, compbuf, (unsigned int)final_comp_len);
    gzflush_ret = gzflush(gz, 0);
    gzclearerr(gz);
    gzclose_ret = gzclose(gz);

    // step 4: Cleanup / validation
    (void)ver;
    (void)input_len;
    (void)dict_len;
    (void)final_comp_len;
    (void)decomp_len;
    (void)ret_def_init_src;
    (void)ret_def_call_src;
    (void)ret_def_copy;
    (void)ret_def_call_dst;
    (void)ret_def_end_src;
    (void)ret_def_end_dst;
    (void)ret_inf_init;
    (void)ret_inf_setdict;
    (void)ret_inf_call;
    (void)ret_inf_end;
    (void)gzwrite_ret;
    (void)gzflush_ret;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}