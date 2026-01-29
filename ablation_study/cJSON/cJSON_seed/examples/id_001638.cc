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
//<ID> 1638
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_null", maybe_null);

    // step 2: Configure
    int items_count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "items_count", (double)items_count);
    cJSON *arr_ref = cJSON_CreateArrayReference(s1);
    cJSON_AddItemToObject(root, "array_ref", arr_ref);

    // step 3: Operate & Validate
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_is_string = cJSON_IsString(first_item);
    cJSON_AddNumberToObject(root, "first_is_string", (double)first_is_string);
    char *first_val = cJSON_GetStringValue(first_item);
    cJSON *first_copy = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "first_copy", first_copy);

    // step 4: Cleanup
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);
    char *full = cJSON_Print(root);
    cJSON_free(full);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}