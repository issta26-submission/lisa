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
//<ID> 707
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
    cJSON *obj_num = cJSON_CreateObject();
    cJSON_AddNumberToObject(obj_num, "value", 3.14159);
    cJSON_AddItemToArray(items, obj_num);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_InsertItemInArray(items, 0, str_item);

    // step 2: Configure
    cJSON *items_ref = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(items_ref, 0);
    cJSON_bool first_is_string = cJSON_IsString(first_item);
    char *first_str = cJSON_GetStringValue(first_item);
    cJSON *second_item = cJSON_GetArrayItem(items_ref, 1);
    cJSON *second_value = cJSON_GetObjectItemCaseSensitive(second_item, "value");

    // step 3: Operate and Validate
    double first_char_code = (double)(first_str[0]);
    cJSON_AddNumberToObject(root, "first_char_code", first_char_code);
    double copied_value = cJSON_GetNumberValue(second_value);
    cJSON_AddNumberToObject(root, "copied_value", copied_value);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}