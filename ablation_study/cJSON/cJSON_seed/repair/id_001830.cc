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
//<ID> 1830
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(3.0));

    // step 2: Configure
    cJSON_AddStringToObject(config, "mode", "test");
    cJSON_AddBoolToObject(config, "enabled", 1);
    cJSON *duplicate = cJSON_Duplicate(root, 1);

    // step 3: Operate and Validate
    int size_before = cJSON_GetArraySize(items);
    cJSON_DeleteItemFromArray(items, 1);
    int size_after = cJSON_GetArraySize(items);
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    cJSON *dup_items = cJSON_GetObjectItem(duplicate, "items");
    int dup_size = cJSON_GetArraySize(dup_items);
    char *printed = cJSON_Print(root);
    cJSON_Minify(printed);
    char first_char = printed ? printed[0] : 'X';
    (void)size_before; (void)size_after; (void)second_value; (void)dup_size; (void)first_char;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(duplicate);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}