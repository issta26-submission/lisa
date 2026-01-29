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
//<ID> 1367
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence test: write with gzwrite, read with gzread, set/get inflate dictionary";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 2: Configure
    const Bytef dict[] = "example-dictionary-for-inflate";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    int setdict_ret = inflateSetDictionary(&istrm, dict, dict_len);
    Bytef retrieved_dict[64];
    memset(retrieved_dict, 0, (int)sizeof(retrieved_dict));
    uInt retrieved_len = (uInt)sizeof(retrieved_dict);
    int getdict_ret = inflateGetDictionary(&istrm, retrieved_dict, &retrieved_len);

    // step 3: Operate
    gzFile gz_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz_w, src, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int close_w = gzclose(gz_w);
    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(src_len + 1));
    memset(read_buf, 0, (int)(src_len + 1));
    int gr = gzread(gz_r, read_buf, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int close_r = gzclose(gz_r);

    // step 4: Validate & Cleanup
    int end_ret = inflateEnd(&istrm);
    (void)init_ret;
    (void)setdict_ret;
    (void)getdict_ret;
    (void)retrieved_len;
    (void)retrieved_dict;
    (void)version;
    (void)gw;
    (void)close_w;
    (void)gr;
    (void)close_r;
    (void)end_ret;
    free(read_buf);
    // API sequence test completed successfully
    return 66;
}