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
//<ID> 904
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload demonstrating deflate/inflate with params and dictionary";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    static Bytef dictionary[] = "example_dictionary_bytes";
    const uInt dict_len = (uInt)(sizeof(dictionary) - 1);

    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure deflate parameters and dictionary, prepare output buffer
    int rc_deflate_params = deflateParams(&dstrm, 5, 0);
    int rc_deflate_set_dict = deflateSetDictionary(&dstrm, dictionary, dict_len);
    dstrm.next_in = (Bytef *)src_buf;
    dstrm.avail_in = (uInt)payload_len;
    uLong comp_bound = deflateBound(&dstrm, (uLong)payload_len);
    Bytef *comp_buf = new Bytef[(comp_bound ? comp_bound : 1)];
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)comp_bound;
    int rc_deflate_call = deflate(&dstrm, 4); // 4 == Z_FINISH
    uLong comp_size = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Initialize inflate, validate it, set dictionary and perform inflation
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_validate_before = inflateValidate(&istrm, 1);
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_size;
    Bytef *uncomp_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = uncomp_buf;
    istrm.avail_out = (uInt)payload_len;
    int rc_inflate_set_dict = inflateSetDictionary(&istrm, dictionary, dict_len);
    int rc_inflate_call = inflate(&istrm, 4); // 4 == Z_FINISH
    int rc_inflate_validate_after = inflateValidate(&istrm, 0);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Cleanup and finalization
    delete [] comp_buf;
    delete [] uncomp_buf;
    (void)rc_deflate_init;
    (void)rc_deflate_params;
    (void)rc_deflate_set_dict;
    (void)rc_deflate_call;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate_validate_before;
    (void)rc_inflate_set_dict;
    (void)rc_inflate_call;
    (void)rc_inflate_validate_after;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}