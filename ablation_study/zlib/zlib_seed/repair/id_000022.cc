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
//<ID> 22
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef dict[] = "sample_dictionary_data";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    Bytef outdict[64];
    uInt outDictLen = (uInt)sizeof(outdict);
    gzFile gzf;
    off64_t seek_pos;
    int ret_init;
    int ret_setdict;
    int ret_getdict;
    int ret_reset;
    int ret_end;
    int ret_gzclose;
    int ret_seek;

    // step 2: Setup (initialize stream and open gzip file)
    memset(&strm, 0, sizeof(strm));
    ret_init = deflateInit_(&strm, 6, zlibVersion(), sizeof(z_stream));
    gzf = gzopen64("test_zlib_api_sequence.tmp.gz", "w+b");

    // step 3: Core operations (set dictionary, retrieve it, reset stream, and seek using retrieved size)
    ret_setdict = deflateSetDictionary(&strm, dict, dictLen);
    ret_getdict = deflateGetDictionary(&strm, outdict, &outDictLen);
    ret_reset = deflateReset(&strm);
    seek_pos = (off64_t)outDictLen;
    ret_seek = gzseek64(gzf, seek_pos, 0);

    // step 4: Cleanup
    ret_end = deflateEnd(&strm);
    ret_gzclose = gzclose(gzf);

    // API sequence test completed successfully
    (void)ret_init; (void)ret_setdict; (void)ret_getdict; (void)ret_reset; (void)ret_seek; (void)ret_end; (void)ret_gzclose; (void)seek_pos;
    return 66;
}