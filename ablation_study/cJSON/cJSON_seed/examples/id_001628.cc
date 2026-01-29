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
//<ID> 1628
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *fruits = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("orange");
    cJSON *s2 = cJSON_CreateString("pear");
    cJSON_AddItemToArray(fruits, s1);
    cJSON_AddItemToArray(fruits, s2);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(fruits, null_item);
    cJSON_AddItemToObject(root, "fruits", fruits);

    // step 2: Configure
    int count = cJSON_GetArraySize(fruits);
    cJSON_AddNumberToObject(root, "fruits_count", (double)count);
    cJSON *ref_item = cJSON_CreateString("kiwi");
    cJSON_AddItemReferenceToArray(fruits, ref_item);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(fruits, 0);
    const char *first_val = cJSON_GetStringValue(first);
    cJSON *first_copy = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    cJSON *maybe_null = cJSON_GetArrayItem(fruits, 2);
    cJSON_bool is_null = cJSON_IsNull(maybe_null);
    cJSON_AddNumberToObject(root, "null_present", (double)is_null);
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);

    // step 4: Cleanup
    char *full = cJSON_Print(root);
    cJSON_free(full);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}