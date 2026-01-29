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
//<ID> 362
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver = zlibVersion();
    z_stream inf_strm;
    Bytef src_buf[] = "Zlib API sequence payload for compress->inflate->dict->gzwrite.";
    uLong src_len = (uLong)(sizeof(src_buf) - 1);
    Bytef comp_buf[2048];
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    Bytef decomp_buf[2048];
    uLongf decomp_len = (uLongf)sizeof(decomp_buf);
    Bytef dict_buf[] = "preset_dictionary_for_inflate";
    uInt dict_len = (uInt)(sizeof(dict_buf) - 1);
    gzFile gzf;
    int ret_compress;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_setdict;
    int ret_inflate_end;
    int ret_gzwrite;
    int ret_gzclose;

    // step 2: Initialize / Configure
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(comp_buf, 0, sizeof(comp_buf));
    memset(decomp_buf, 0, sizeof(decomp_buf));
    comp_len = (uLongf)sizeof(comp_buf);
    ret_compress = compress(comp_buf, &comp_len, (const Bytef *)src_buf, src_len);
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)decomp_len;

    // step 3: Operate / Validate
    ret_inflate_call = inflate(&inf_strm, 0);
    ret_setdict = inflateSetDictionary(&inf_strm, dict_buf, dict_len);
    ret_inflate_end = inflateEnd(&inf_strm);

    // step 4: Cleanup - write compressed data to gz file and close
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gzf, (voidpc)comp_buf, (unsigned int)comp_len);
    ret_gzclose = gzclose(gzf);

    (void)ver;
    (void)src_len;
    (void)decomp_len;
    (void)ret_compress;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_setdict;
    (void)ret_inflate_end;
    (void)ret_gzwrite;
    (void)ret_gzclose;
    // API sequence test completed successfully
    return 66;
}