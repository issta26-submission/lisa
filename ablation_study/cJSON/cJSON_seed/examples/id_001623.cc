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
//<ID> 1623
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *s1 = cJSON_CreateString("apple");
    cJSON *s2 = cJSON_CreateString("banana");
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(items, null_item);

    // step 2: Configure
    int size = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "items_count", (double)size);
    cJSON_bool is_null_flag = cJSON_IsNull(null_item);
    cJSON_AddNumberToObject(root, "null_is_null_flag", (double)is_null_flag);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    char *first_val = cJSON_GetStringValue(first);
    cJSON *first_copy = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    cJSON *ref_item = cJSON_CreateString("apple_ref");
    cJSON_AddItemReferenceToArray(items, ref_item);

    // step 4: Cleanup
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);
    char *full = cJSON_Print(root);
    cJSON_free(full);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}