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
//<ID> 1360
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "Zlib API sequence test: inflateInit_, inflateSetDictionary, inflateGetDictionary, gzwrite, gzread";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = inflateInit_(&strm, version, (int)sizeof(z_stream));

    // step 2: Configure
    const Bytef dictionary[] = "example_dictionary_for_inflate";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);
    int setdict_ret = inflateSetDictionary(&strm, dictionary, dict_len);
    Bytef *retrieved_dict = (Bytef *)malloc((size_t)128);
    memset(retrieved_dict, 0, 128);
    uInt retrieved_len = (uInt)128;
    int getdict_ret = inflateGetDictionary(&strm, retrieved_dict, &retrieved_len);

    // step 3: Operate
    gzFile gz_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int write_ret = gzwrite(gz_w, src_data, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int close_w = gzclose(gz_w);
    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(src_len + 1));
    memset(read_buf, 0, (int)(src_len + 1));
    int read_ret = gzread(gz_r, read_buf, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));

    // step 4: Validate & Cleanup
    (void)init_ret;
    (void)setdict_ret;
    (void)getdict_ret;
    (void)write_ret;
    (void)close_w;
    (void)read_ret;
    (void)retrieved_len;
    (void)version;
    int end_ret = inflateEnd(&strm);
    int close_r = gzclose(gz_r);
    free(retrieved_dict);
    free(read_buf);
    // API sequence test completed successfully
    return 66;
}