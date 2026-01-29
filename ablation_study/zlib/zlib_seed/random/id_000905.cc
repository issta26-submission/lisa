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
//<ID> 905
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload demonstrating deflate/deflateParams and inflateValidate/inflateSetDictionary";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, (uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    dstrm.next_in = (Bytef *)src_buf;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(bound <= (uLong)~0u ? (uInt)bound : (uInt)~0u);

    // step 2: Configure deflate parameters and perform compression
    int rc_deflate_params = deflateParams(&dstrm, 9, 0);
    int rc_deflate_call = deflate(&dstrm, 4); // Z_FINISH == 4
    uLong comp_len = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Initialize inflate, set dictionary, validate and decompress
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len <= (uLong)~0u ? comp_len : (uLong)~0u);
    uLong out_bound = payload_len;
    Bytef *out_buf = new Bytef[(out_bound ? out_bound : 1)];
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(out_bound <= (uLong)~0u ? (uInt)out_bound : (uInt)~0u);
    static Bytef preset_dict[] = "preset_dictionary_example";
    uInt preset_dict_len = (uInt)(sizeof(preset_dict) - 1);
    int rc_inflate_set_dict = inflateSetDictionary(&istrm, preset_dict, preset_dict_len);
    int rc_inflate_validate = inflateValidate(&istrm, 15);
    int rc_inflate_call = inflate(&istrm, 4); // Z_FINISH == 4
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Cleanup and finalization
    delete [] comp_buf;
    delete [] out_buf;
    (void)rc_deflate_init;
    (void)rc_deflate_params;
    (void)rc_deflate_call;
    (void)comp_len;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate_set_dict;
    (void)rc_inflate_validate;
    (void)rc_inflate_call;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}