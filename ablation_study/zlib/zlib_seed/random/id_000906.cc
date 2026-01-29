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
//<ID> 906
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib sequence payload for deflate/inflate with dictionary and validation";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong comp_bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(comp_bound ? comp_bound : 1)];
    Bytef *out_buf = new Bytef[(payload_len ? payload_len : 1)];
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)src_buf;
    dstrm.avail_in = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(comp_bound <= (uLong)~0u ? comp_bound : (uLong)~0u);

    // step 2: Configure compression parameters and perform deflate
    int rc_deflate_params = deflateParams(&dstrm, 6, 0);
    int rc_deflate_call = deflate(&dstrm, 4); // 4 == Z_FINISH

    // step 3: Initialize inflate, set a dictionary and validate, then inflate
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    static Bytef dictionary[] = "preset_dictionary_example";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);
    int rc_inflate_set_dict = inflateSetDictionary(&istrm, dictionary, dict_len);
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(dstrm.total_out <= (uLong)~0u ? dstrm.total_out : (uLong)~0u);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate_call = inflate(&istrm, 0); // 0 == Z_NO_FLUSH
    int rc_inflate_validate = inflateValidate(&istrm, 15);

    // step 4: Cleanup and finalization
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] comp_buf;
    delete [] out_buf;
    (void)rc_deflate_init;
    (void)rc_deflate_params;
    (void)rc_deflate_call;
    (void)rc_inflate_init;
    (void)rc_inflate_set_dict;
    (void)rc_inflate_call;
    (void)rc_inflate_validate;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}