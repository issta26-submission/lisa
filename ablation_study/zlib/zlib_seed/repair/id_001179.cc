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
//<ID> 1179
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize data and compute checksums
    const Bytef source[] = "zlib sequence payload demonstrating adler32/adler32_combine64/inflateSetDictionary/deflateEnd usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const Bytef dictionary[] = "example_dictionary_data";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);
    uLong ad1 = adler32(0UL, source, (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len));
    uLong ad2 = adler32(0UL, dictionary, dict_len);
    uLong combined_ad = adler32_combine64(ad1, ad2, (off64_t)dict_len);

    // step 2: Initialize inflate stream and configure with input and dictionary
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    inf_strm.next_in = (Bytef *)source;
    inf_strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    int infl_setdict_ret = inflateSetDictionary(&inf_strm, dictionary, dict_len);

    // step 3: Prepare a deflate stream object and perform cleanup via deflateEnd
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_end_ret = deflateEnd(&def_strm);

    // step 4: Finalize, ensure values are referenced and return success
    (void)ad1;
    (void)ad2;
    (void)combined_ad;
    (void)infl_setdict_ret;
    (void)def_end_ret;
    (void)source_len;
    (void)dict_len;
    (void)source;
    (void)dictionary;
    (void)&inf_strm;
    (void)&def_strm;
    // API sequence test completed successfully
    return 66;
}