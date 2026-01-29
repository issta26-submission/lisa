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
//<ID> 23
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialization
    z_stream dstrm;
    Bytef getbuf[64];
    unsigned char dict[] = "sample_dictionary_for_deflate";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    uInt getlen = (uInt)sizeof(getbuf);
    gzFile gzf;
    off64_t seek_pos;
    int ret_deflate_init;
    int ret_set_dict1;
    int ret_get_dict;
    int ret_reset;
    int ret_set_dict2;
    int ret_deflate_end;
    int ret_gzclose;
    memset(&dstrm, 0, sizeof(dstrm));
    ret_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), sizeof(z_stream));
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "w+b");

    // step 2: Configure (set a dictionary for deflate)
    ret_set_dict1 = deflateSetDictionary(&dstrm, (const Bytef *)dict, dictLen);

    // step 3: Operate & Validate (retrieve dictionary, reset stream, re-apply retrieved dictionary, seek gz file)
    ret_get_dict = deflateGetDictionary(&dstrm, getbuf, &getlen);
    ret_reset = deflateReset(&dstrm);
    ret_set_dict2 = deflateSetDictionary(&dstrm, getbuf, getlen);
    seek_pos = gzseek64(gzf, (off64_t)0, 0);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&dstrm);
    ret_gzclose = gzclose(gzf);
    (void)ret_deflate_init; (void)ret_set_dict1; (void)ret_get_dict; (void)ret_reset; (void)ret_set_dict2;
    (void)seek_pos; (void)ret_deflate_end; (void)ret_gzclose; (void)dictLen; (void)getlen;

    // API sequence test completed successfully
    return 66;
}