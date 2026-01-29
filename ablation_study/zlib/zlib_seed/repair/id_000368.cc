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
//<ID> 368
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver;
    Bytef src_buf[] = "Zlib API sequence payload demonstrating version, dictionary, inflate and gz write.";
    uLong src_len = (uLong)(sizeof(src_buf) - 1);
    Bytef comp_buf[1024];
    Bytef decomp_buf[1024];
    uLongf comp_len_f;
    uLong decomp_len;
    z_stream inf_strm;
    Bytef dict_buf[] = "preset_dictionary_for_inflate";
    uInt dict_len = (uInt)(sizeof(dict_buf) - 1);
    int ret_compress;
    int ret_inflate_init;
    int ret_inflate_setdict;
    int ret_inflate_call;
    int ret_inflate_end;
    gzFile gz_wr;
    int ret_gzwrite;
    int ret_gzclose;

    // step 2: Setup / initialization
    ver = zlibVersion();
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(comp_buf, 0, sizeof(comp_buf));
    memset(decomp_buf, 0, sizeof(decomp_buf));
    comp_len_f = (uLongf)sizeof(comp_buf);
    ret_compress = compress(comp_buf, &comp_len_f, (const Bytef *)src_buf, src_len);

    // step 3: Core operations - initialize inflate, set dictionary, perform inflate
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len_f;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate_setdict = inflateSetDictionary(&inf_strm, dict_buf, dict_len);
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inflate_end = inflateEnd(&inf_strm);

    // step 4: Operate / Validate / Cleanup - write compressed data to gz and close
    gz_wr = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gz_wr, (voidpc)comp_buf, (unsigned int)comp_len_f);
    ret_gzclose = gzclose(gz_wr);

    (void)ver;
    (void)src_len;
    (void)decomp_len;
    (void)ret_compress;
    (void)ret_inflate_init;
    (void)ret_inflate_setdict;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)ret_gzwrite;
    (void)ret_gzclose;
    // API sequence test completed successfully
    return 66;
}