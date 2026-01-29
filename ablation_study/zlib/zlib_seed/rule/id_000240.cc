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
//<ID> 240
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    unsigned char input[512];
    memset(input, 'X', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    const char preset_dict[] = "preset_dictionary_123";
    uInt preset_dict_len = (uInt)(sizeof(preset_dict) - 1);

    // step 2: Setup
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateSetDictionary(&dstrm, (const Bytef *)preset_dict, preset_dict_len);
    uLong compBound = deflateBound(&dstrm, inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    Bytef *outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);

    // step 3: Core operations
    uInt dict_copy_len = (uInt)64;
    Bytef *dict_copy = (Bytef *)malloc((size_t)dict_copy_len);
    memset(dict_copy, 0, (size_t)dict_copy_len);
    deflateGetDictionary(&dstrm, dict_copy, &dict_copy_len);
    deflateResetKeep(&dstrm);

    dstrm.next_in = (Bytef *)input;
    dstrm.avail_in = (uInt)inputLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    deflate(&dstrm, 0);
    uLong compressed_size = dstrm.total_out;

    uLongf destLen = (uLongf)inputLen;
    uncompress(outBuf, &destLen, compBuf, compressed_size);

    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, "example metadata");
    gzseek64(gz, 0, 0);
    gzclose(gz);

    // step 4: Validate & Cleanup
    deflateEnd(&dstrm);
    free(compBuf);
    free(outBuf);
    free(dict_copy);

    // API sequence test completed successfully
    return 66;
}