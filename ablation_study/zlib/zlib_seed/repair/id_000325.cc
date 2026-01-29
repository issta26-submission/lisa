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
//<ID> 325
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver;
    z_stream def_strm;
    z_stream def_copy;
    z_stream inf_strm;
    Bytef input_buf[] = "Zlib sequence payload for deflate/inflate and gz operations.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef dict_buf[] = "preset-dictionary";
    uInt dict_len = (uInt)(sizeof(dict_buf) - 1);
    Bytef compbuf[32768];
    Bytef decompbuf[32768];
    uLong comp_len = 0;
    uLong decomp_len = 0;
    int ret_def_init;
    int ret_def_copy;
    int ret_def_call_orig;
    int ret_def_call_copy;
    int ret_def_end_orig;
    int ret_def_end_copy;
    int ret_inf_init;
    int ret_inf_setdict;
    int ret_inf_call;
    int ret_inf_end;
    gzFile gz;
    int gz_write_ret;
    int gz_flush_ret;
    int gz_close_ret;

    // step 2: Setup / initialize
    ver = zlibVersion();
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&def_copy, 0, sizeof(def_copy));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(decompbuf, 0, sizeof(decompbuf));
    ret_def_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_def_copy = deflateCopy(&def_copy, &def_strm);
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    def_copy.next_in = input_buf;
    def_copy.avail_in = (uInt)input_len;
    def_copy.next_out = compbuf;
    def_copy.avail_out = (uInt)sizeof(compbuf);

    // step 3: Core operations - compress with original and copy, then inflate with dictionary
    ret_def_call_orig = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_def_call_copy = deflate(&def_copy, 4);
    ret_def_end_orig = deflateEnd(&def_strm);
    ret_def_end_copy = deflateEnd(&def_copy);
    ret_inf_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decompbuf;
    inf_strm.avail_out = (uInt)sizeof(decompbuf);
    ret_inf_setdict = inflateSetDictionary(&inf_strm, (const Bytef *)dict_buf, dict_len);
    ret_inf_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inf_end = inflateEnd(&inf_strm);

    // step 4: GZ file operations and cleanup/validation
    gz = gzopen("test_zlib_api_sequence.gz", "wb+");
    gz_write_ret = gzwrite(gz, (voidpc)compbuf, (unsigned int)comp_len);
    gz_flush_ret = gzflush(gz, 2);
    gzclearerr(gz);
    gz_close_ret = gzclose(gz);

    (void)ver;
    (void)input_len;
    (void)dict_len;
    (void)comp_len;
    (void)decomp_len;
    (void)ret_def_init;
    (void)ret_def_copy;
    (void)ret_def_call_orig;
    (void)ret_def_call_copy;
    (void)ret_def_end_orig;
    (void)ret_def_end_copy;
    (void)ret_inf_init;
    (void)ret_inf_setdict;
    (void)ret_inf_call;
    (void)ret_inf_end;
    (void)gz_write_ret;
    (void)gz_flush_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}