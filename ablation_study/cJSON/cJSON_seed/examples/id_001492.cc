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
//<ID> 1492
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON_AddItemToObject(root, "items", items);
    // Use a safe duplicate for the "items_ref" entry instead of creating a shared reference
    cJSON *items_ref = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_ref", items_ref);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON *first_elem = cJSON_GetArrayItem(got_items, 0);
    char *first_str = cJSON_GetStringValue(first_elem);
    cJSON_bool got_is_array = cJSON_IsArray(got_items);
    (void)first_str;
    (void)got_is_array;

    // step 3: Operate & Validate
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *dup_items_ref = cJSON_GetObjectItem(dup, "items_ref");
    cJSON_bool equal_refs = cJSON_Compare(got_items, dup_items_ref, 1);
    int arr_size = cJSON_GetArraySize(got_items);
    (void)equal_refs;
    (void)arr_size;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}