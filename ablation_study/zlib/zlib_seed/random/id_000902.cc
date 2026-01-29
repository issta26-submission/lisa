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
//<ID> 902
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "example payload for zlib deflate/inflate with preset dictionary and validation";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    static Bytef preset_dict[] = "preset_dictionary_for_zlib_test";
    const uInt dict_len = (uInt)(sizeof(preset_dict) - 1);
    uLong comp_bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(comp_bound ? comp_bound : 1)];
    Bytef *out_buf = new Bytef[(payload_len ? payload_len : 1)];

    // step 2: Initialize and configure deflate, set dictionary, and perform compression
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    int rc_deflate_set_dict = deflateSetDictionary(&dstrm, preset_dict, dict_len);
    int rc_deflate_params = deflateParams(&dstrm, 5, 0); // change level and keep default strategy
    dstrm.next_in = (Bytef *)src_buf;
    dstrm.avail_in = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(comp_bound <= (uLong)~0u ? comp_bound : (uLong)~0u);
    int rc_deflate = deflate(&dstrm, 4); // Z_FINISH
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Initialize inflate, validate, set dictionary and perform inflation
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_validate = inflateValidate(&istrm, 1);
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(dstrm.total_out <= (uLong)~0u ? dstrm.total_out : (uLong)~0u);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate_first = inflate(&istrm, 4); // attempt inflate (may require dictionary)
    int rc_inflate_set_dict = inflateSetDictionary(&istrm, preset_dict, dict_len);
    int rc_inflate_second = inflate(&istrm, 4); // attempt again after setting dictionary
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Cleanup and finalization
    delete [] comp_buf;
    delete [] out_buf;
    (void)rc_deflate_init;
    (void)rc_deflate_set_dict;
    (void)rc_deflate_params;
    (void)rc_deflate;
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