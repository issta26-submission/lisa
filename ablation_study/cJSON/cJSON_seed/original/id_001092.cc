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
//<ID> 1092
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
    cJSON *n0 = cJSON_CreateNumber(1.1);
    cJSON *n1 = cJSON_CreateNumber(2.2);
    cJSON *n2 = cJSON_CreateNumber(3.3);
    cJSON_AddItemToArray(items, n0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON *name = cJSON_CreateString("original");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *optional = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", optional);

    // step 2: Configure
    cJSON *new_name = cJSON_CreateString("replaced");
    cJSON_ReplaceItemViaPointer(root, name, new_name);
    cJSON_bool items_are_array = cJSON_IsArray(items);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *replacement_first = cJSON_CreateNumber(4.4);
    cJSON_ReplaceItemViaPointer(items, first_item, replacement_first);

    // step 3: Operate
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    cJSON_bool optional_was_null = cJSON_IsNull(optional);
    char *unformatted = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_PrintPreallocated(root, buffer, 256, 1);

    // step 4: Validate & Cleanup
    (void)items_are_array;
    (void)second_value;
    (void)optional_was_null;
    cJSON_free(unformatted);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}