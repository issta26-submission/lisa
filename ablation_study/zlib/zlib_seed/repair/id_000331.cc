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
//<ID> 331
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver = zlibVersion();
    z_stream def_strm;
    z_stream inf_strm;
    Bytef input_buf[] = "Zlib API sequence payload for deflateReset, deflateGetDictionary, inflateEnd, gzopen64 and gzopen.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    uInt input_len_u = (uInt)input_len;
    Bytef compbuf[32768];
    Bytef decompbuf[32768];
    Bytef dictbuf[512];
    uLong comp_len = 0;
    uLong decomp_len = 0;
    uInt dictlen = 0;
    int ret_def_init = 0;
    int ret_def_first = 0;
    int ret_def_reset = 0;
    int ret_def_second = 0;
    int ret_def_getdict = 0;
    int ret_def_end = 0;
    int ret_inf_init = 0;
    int ret_inf_call = 0;
    int ret_inf_end = 0;
    gzFile gz1 = (gzFile)0;
    gzFile gz2 = (gzFile)0;
    int gz1_write = 0;
    int gz2_write = 0;
    int gz1_close = 0;
    int gz2_close = 0;

    // step 2: Setup / initialization
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(decompbuf, 0, sizeof(decompbuf));
    memset(dictbuf, 0, sizeof(dictbuf));
    ret_def_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = input_buf;
    def_strm.avail_in = input_len_u;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);

    // step 3: Core operations (compress, reset, compress again, get dictionary, inflate, cleanup)
    ret_def_first = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_def_reset = deflateReset(&def_strm);
    def_strm.next_in = input_buf;
    def_strm.avail_in = input_len_u;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    ret_def_second = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_def_getdict = deflateGetDictionary(&def_strm, dictbuf, &dictlen);
    ret_def_end = deflateEnd(&def_strm);
    ret_inf_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decompbuf;
    inf_strm.avail_out = (uInt)sizeof(decompbuf);
    ret_inf_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inf_end = inflateEnd(&inf_strm);

    // step 4: File writes & final cleanup
    gz1 = gzopen64("test_zlib_api_sequence_64.gz", "wb");
    gz2 = gzopen("test_zlib_api_sequence.gz", "wb");
    gz1_write = gzwrite(gz1, (voidpc)compbuf, (unsigned int)comp_len);
    gz2_write = gzwrite(gz2, (voidpc)decompbuf, (unsigned int)decomp_len);
    gz1_close = gzclose(gz1);
    gz2_close = gzclose(gz2);

    (void)ver;
    (void)input_len;
    (void)input_len_u;
    (void)comp_len;
    (void)decomp_len;
    (void)dictlen;
    (void)ret_def_init;
    (void)ret_def_first;
    (void)ret_def_reset;
    (void)ret_def_second;
    (void)ret_def_getdict;
    (void)ret_def_end;
    (void)ret_inf_init;
    (void)ret_inf_call;
    (void)ret_inf_end;
    (void)gz1_write;
    (void)gz2_write;
    (void)gz1_close;
    (void)gz2_close;
    // API sequence test completed successfully
    return 66;
}