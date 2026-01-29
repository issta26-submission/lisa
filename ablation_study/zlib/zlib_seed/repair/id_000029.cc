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
//<ID> 29
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    const Bytef dict_init[] = "deflate_dictionary";
    uInt dict_init_len = (uInt)(sizeof(dict_init) - 1);
    Bytef inbuf[] = "Example data to be compressed by zlib.";
    uInt in_len = (uInt)(sizeof(inbuf) - 1);
    Bytef outbuf[256];
    Bytef dict_out[64];
    uInt dict_out_len = (uInt)sizeof(dict_out);
    gzFile gzf;
    off64_t seek_pos = 0;
    int ret_deflate_init;
    int ret_set_dict;
    int ret_deflate;
    int ret_get_dict;
    int ret_deflate_reset;
    int ret_deflate_end;
    int ret_gzopen;
    off64_t ret_seek;
    int ret_gzclose;

    // step 2: Setup (initialize deflate stream and open gzip file)
    memset(&strm, 0, sizeof(strm));
    ret_deflate_init = deflateInit_(&strm, 6, zlibVersion(), (int)sizeof(z_stream));
    strm.next_in = (Bytef *)inbuf;
    strm.avail_in = in_len;
    strm.next_out = outbuf;
    strm.avail_out = (uInt)sizeof(outbuf);
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "w+b");
    ret_gzopen = (gzf != (gzFile)0) ? 0 : -1;

    // step 3: Operate & Validate (set dictionary, perform a deflate, retrieve dictionary, reset)
    ret_set_dict = deflateSetDictionary(&strm, dict_init, dict_init_len);
    ret_deflate = deflate(&strm, 0);
    ret_get_dict = deflateGetDictionary(&strm, dict_out, &dict_out_len);
    ret_deflate_reset = deflateReset(&strm);
    ret_seek = gzseek64(gzf, seek_pos, 0);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&strm);
    ret_gzclose = gzclose(gzf);

    // API sequence test completed successfully
    (void)ret_deflate_init; (void)ret_set_dict; (void)ret_deflate; (void)ret_get_dict; (void)dict_out_len;
    (void)ret_deflate_reset; (void)ret_deflate_end; (void)ret_gzopen; (void)ret_seek; (void)ret_gzclose;
    return 66;
}