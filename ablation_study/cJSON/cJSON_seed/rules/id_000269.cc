#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 269
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *orig = cJSON_CreateArray();
    cJSON *refs = cJSON_CreateArray();
    cJSON *b1 = cJSON_CreateTrue();
    cJSON *b2 = cJSON_CreateTrue();
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    int size_orig = 0;
    int size_refs = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "original_array", orig);
    cJSON_AddItemToObject(root, "reference_array", refs);
    cJSON_AddItemToArray(orig, b1);
    cJSON_AddItemToArray(refs, b2);
    cJSON_AddItemReferenceToArray(refs, b1);

    // step 3: Operate / Validate
    size_orig = cJSON_GetArraySize(orig);
    size_refs = cJSON_GetArraySize(refs);
    equal = cJSON_Compare(b1, b2, 1);
    buffer[0] = (char)('0' + (size_orig % 10));
    (void)size_refs;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}