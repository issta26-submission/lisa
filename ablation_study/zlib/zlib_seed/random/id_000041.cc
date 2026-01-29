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
//<ID> 41
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream strm_def;
    z_stream strm_infl;
    memset(&strm_def, 0, sizeof(z_stream));
    memset(&strm_infl, 0, sizeof(z_stream));
    const char dict_c[] = "example_dictionary_data_for_zlib";
    const Bytef *dict = (const Bytef *)dict_c;
    uInt dict_len = (uInt)(sizeof(dict_c) - 1);
    const char payload[] = "payload_data_for_zlib_operations";
    const Bytef *payload_b = (const Bytef *)payload;
    uInt payload_len = (uInt)(sizeof(payload) - 1);

    // step 2: Setup (initialize deflate and inflate, set dictionaries)
    const char *version = zlibVersion();
    int rc_deflateInit = deflateInit_(&strm_def, 6, version, (int)sizeof(z_stream));
    strm_def.next_in = (Bytef *)payload_b;
    strm_def.avail_in = payload_len;
    int rc_deflateSetDict = deflateSetDictionary(&strm_def, dict, dict_len);
    int rc_inflateInit2 = inflateInit2_(&strm_infl, 15, version, (int)sizeof(z_stream));
    strm_infl.next_in = (Bytef *)payload_b;
    strm_infl.avail_in = payload_len;
    int rc_inflateSetDict = inflateSetDictionary(&strm_infl, dict, dict_len);

    // step 3: Operate -> Validate (open gzip file, obtain offset, write status)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    off64_t offset = gzoffset64(gz);
    int rc_printf = gzprintf(gz, "deflateInit=%d deflateSetDict=%d inflateInit2=%d inflateSetDict=%d off64=%lld\n",
                             rc_deflateInit, rc_deflateSetDict, rc_inflateInit2, rc_inflateSetDict, (long long)offset);

    // step 4: Cleanup
    int rc_deflateEnd = deflateEnd(&strm_def);
    int rc_inflateEnd = inflateEnd(&strm_infl);
    int rc_gzclose = gzclose(gz);
    (void)payload_b; (void)payload_len; (void)rc_printf; (void)rc_deflateEnd; (void)rc_inflateEnd; (void)rc_gzclose;

    // API sequence test completed successfully
    return 66;
}