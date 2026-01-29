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
//<ID> 1624
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", arr);
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, n1);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_null", null_item);
    cJSON *s_owner = cJSON_CreateString("owner");
    cJSON_AddItemToObject(root, "owner", s_owner);
    cJSON_AddItemReferenceToArray(arr, s_owner);

    // step 2: Configure
    cJSON *got_null = cJSON_GetObjectItem(root, "maybe_null");
    cJSON_bool is_null = cJSON_IsNull(got_null);
    cJSON_AddNumberToObject(root, "null_is_null", (double)is_null);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *first_val = cJSON_GetStringValue(first);
    cJSON *first_copy = cJSON_CreateString(first_val ? first_val : "");
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);

    // step 4: Cleanup
    char *full = cJSON_Print(root);
    cJSON_free(full);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}