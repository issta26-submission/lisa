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
//<ID> 509
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers (create sample data and allocate buffers)
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compSimple = (Bytef *)malloc((size_t)bound);
    memset(compSimple, 0, (size_t)bound);
    uLongf compSimpleLen = (uLongf)bound;
    Bytef * defComp = (Bytef *)malloc((size_t)bound);
    memset(defComp, 0, (size_t)bound);

    // step 2: Initialize & Configure (initialize deflate stream)
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = defComp;
    defStrm.avail_out = (uInt)bound;

    // step 3: Operate & Validate (use compress API and perform deflate, then write original data to a gz file)
    compress(compSimple, &compSimpleLen, (const Bytef *)input, inputLen);
    deflate(&defStrm, Z_FINISH);
    uLong defLen = (uLong)((uLong)bound - (uLong)defStrm.avail_out);
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzwrite(gz, input, (unsigned int)inputLen);
    gzclose(gz);

    // step 4: Cleanup (end deflate and free buffers)
    deflateEnd(&defStrm);
    free(compSimple);
    free(defComp);

    return 66; // API sequence test completed successfully
}