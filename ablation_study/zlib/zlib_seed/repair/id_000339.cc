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
//<ID> 339
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
    z_stream inf_strm;
    Bytef input_buf[] = "Zlib API sequence payload demonstrating deflate/inflate, dictionary retrieval and gz file usage.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    uInt input_len_u = (uInt)input_len;
    Bytef compbuf[16384];
    Bytef decompbuf[16384];
    Bytef dict_buf[1024];
    uInt dict_len;
    uLong comp_len = 0;
    uLong decomp_len = 0;
    int ret_def_init;
    int ret_def_call;
    int ret_def_getdict;
    int ret_def_reset;
    int ret_def_end;
    int ret_inf_init;
    int ret_inf_call;
    int ret_inf_end;
    gzFile gz1;
    gzFile gz2;
    int gzwrite_ret1;
    int gzwrite_ret2;
    int gzclose_ret1;
    int gzclose_ret2;

    // step 2: Setup / initialization
    ver = zlibVersion();
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(decompbuf, 0, sizeof(decompbuf));
    memset(dict_buf, 0, sizeof(dict_buf));
    ret_def_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)input_buf;
    def_strm.avail_in = (uInt)input_len_u;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);

    // step 3: Core operations - compress, get dictionary, reset, inflate, gz files
    ret_def_call = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    dict_len = (uInt)sizeof(dict_buf);
    ret_def_getdict = deflateGetDictionary(&def_strm, dict_buf, &dict_len);
    ret_def_reset = deflateReset(&def_strm);
    ret_inf_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decompbuf;
    inf_strm.avail_out = (uInt)sizeof(decompbuf);
    ret_inf_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    gz1 = gzopen("test_zlib_api_sequence.gz", "wb+");
    gz2 = gzopen64("test_zlib_api_sequence_64.gz", "wb+");
    gzwrite_ret1 = gzwrite(gz1, (voidpc)compbuf, (unsigned int)comp_len);
    gzwrite_ret2 = gzwrite(gz2, (voidpc)dict_buf, (unsigned int)dict_len);

    // step 4: Cleanup / validation
    ret_inf_end = inflateEnd(&inf_strm);
    ret_def_end = deflateEnd(&def_strm);
    gzclose_ret1 = gzclose(gz1);
    gzclose_ret2 = gzclose(gz2);
    (void)ver;
    (void)input_len;
    (void)input_len_u;
    (void)comp_len;
    (void)decomp_len;
    (void)ret_def_init;
    (void)ret_def_call;
    (void)ret_def_getdict;
    (void)ret_def_reset;
    (void)ret_def_end;
    (void)ret_inf_init;
    (void)ret_inf_call;
    (void)ret_inf_end;
    (void)gzwrite_ret1;
    (void)gzwrite_ret2;
    (void)gzclose_ret1;
    (void)gzclose_ret2;
    // API sequence test completed successfully
    return 66;
}