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
//<ID> 1364
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef dictionary[] = "zlib-inflate-dictionary-example";
    uInt dictionary_len = (uInt)(sizeof(dictionary) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *version = zlibVersion();
    int init_ret = inflateInit_(&strm, version, (int)sizeof(z_stream));

    // step 2: Configure
    int set_ret = inflateSetDictionary(&strm, dictionary, dictionary_len);
    uInt got_len = dictionary_len;
    Bytef *got_dictionary = (Bytef *)malloc((size_t)got_len);
    memset(got_dictionary, 0, (int)got_len);
    int get_ret = inflateGetDictionary(&strm, got_dictionary, &got_len);

    // step 3: Operate
    gzFile gz_w = gzopen("test_zlib_api_sequence_dict.gz", "wb");
    int gw = gzwrite(gz_w, got_dictionary, (unsigned int)(got_len));
    int close_w = gzclose(gz_w);
    gzFile gz_r = gzopen("test_zlib_api_sequence_dict.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(got_len + 1));
    memset(read_buf, 0, (int)(got_len + 1));
    int gr = gzread(gz_r, read_buf, (unsigned int)(got_len));
    int close_r = gzclose(gz_r);

    // step 4: Validate & Cleanup
    int end_ret = inflateEnd(&strm);
    (void)init_ret;
    (void)set_ret;
    (void)get_ret;
    (void)gw;
    (void)close_w;
    (void)gr;
    (void)close_r;
    (void)end_ret;
    (void)version;
    free(got_dictionary);
    free(read_buf);
    // API sequence test completed successfully
    return 66;
}