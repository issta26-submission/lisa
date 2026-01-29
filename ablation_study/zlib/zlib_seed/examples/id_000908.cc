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
//<ID> 908
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "Example payload for zlib deflate/inflate with preset dictionary";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    static Bytef preset_dict[] = "preset_dictionary_for_zlib_example";
    const uInt dict_len = (uInt)(sizeof(preset_dict) - 1);
    uLong comp_bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(comp_bound ? comp_bound : 1)];
    Bytef *out_buf = new Bytef[(payload_len ? payload_len : 1)];

    // step 2: Setup and configure deflate stream
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, Z_DEFAULT_COMPRESSION, zlibVersion(), (int)sizeof(z_stream));
    int rc_deflate_params = deflateParams(&dstrm, 6, Z_DEFAULT_STRATEGY);
    int rc_deflate_set_dict = deflateSetDictionary(&dstrm, preset_dict, dict_len);

    // step 3: Operate (deflate) -> validate and inflate with provided dictionary
    dstrm.next_in = (Bytef *)src_buf;
    dstrm.avail_in = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(comp_bound <= (uLong)~0u ? comp_bound : (uLong)~0u);
    int rc_deflate_finish = deflate(&dstrm, Z_FINISH);
    uLong compressed_len = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(compressed_len <= (uLong)~0u ? compressed_len : (uLong)~0u);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate_validate = inflateValidate(&istrm, 1);
    int rc_inflate_first = inflate(&istrm, Z_FINISH);
    int rc_inflate_set_dict = inflateSetDictionary(&istrm, preset_dict, dict_len);
    int rc_inflate_second = inflate(&istrm, Z_FINISH);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Cleanup and finalization
    delete [] comp_buf;
    delete [] out_buf;
    (void)rc_deflate_init;
    (void)rc_deflate_params;
    (void)rc_deflate_set_dict;
    (void)rc_deflate_finish;
    (void)compressed_len;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate_validate;
    (void)rc_inflate_first;
    (void)rc_inflate_set_dict;
    (void)rc_inflate_second;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}