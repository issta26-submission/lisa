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
//<ID> 1107
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *entry = cJSON_CreateObject();
    cJSON_AddStringToObject(entry, "id", "one");
    cJSON_AddNumberToObject(entry, "value", 123.0);
    cJSON *arr_ref = cJSON_CreateArrayReference(entry);
    cJSON_AddItemToObject(root, "refs", arr_ref);
    cJSON *nested = cJSON_AddObjectToObject(root, "nested");
    cJSON *flag = cJSON_AddFalseToObject(nested, "flag");

    // step 2: Configure
    cJSON_bool is_bool = cJSON_IsBool(flag);
    cJSON_bool is_false = cJSON_IsFalse(flag);
    cJSON_AddNumberToObject(root, "status", (double)(is_bool + is_false));

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    cJSON *got_refs = cJSON_GetObjectItem(root, "refs");
    int got_size = cJSON_GetArraySize(got_refs);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = (char)('0' + (got_size & 0xF));
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(entry);
    cJSON_free(scratch);
    // API sequence test completed successfully
    return 66;
}