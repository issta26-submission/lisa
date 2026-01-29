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
//<ID> 403
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    Bytef dict[] = "example_dictionary_for_deflateSetDictionary";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    Bytef outdict[128];
    uInt outdict_len = (uInt)sizeof(outdict);
    int init_ret;
    int setdict_ret;
    int getdict_ret;
    gzFile gz_wr;
    int gzbuffer_ret;
    int gzwrite_ret;
    off64_t offset64;
    int eof_ret;
    int gzclose_ret;
    int end_ret;

    // step 2: Setup / Configure
    init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    setdict_ret = deflateSetDictionary(&strm, dict, dict_len);
    getdict_ret = deflateGetDictionary(&strm, outdict, &outdict_len);

    // step 3: Operate
    gz_wr = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzbuffer_ret = gzbuffer(gz_wr, 8192);
    gzwrite_ret = gzwrite(gz_wr, (voidpc)outdict, (unsigned int)outdict_len);
    offset64 = gzoffset64(gz_wr);
    eof_ret = gzeof(gz_wr);
    gzclose_ret = gzclose(gz_wr);

    // step 4: Validate / Cleanup
    end_ret = deflateEnd(&strm);
    (void)ver;
    (void)init_ret;
    (void)setdict_ret;
    (void)getdict_ret;
    (void)outdict_len;
    (void)gzbuffer_ret;
    (void)gzwrite_ret;
    (void)offset64;
    (void)eof_ret;
    (void)gzclose_ret;
    (void)end_ret;
    // API sequence test completed successfully
    return 66;
}