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
//<ID> 988
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);

    // step 2: Configure
    cJSON *first = cJSON_CreateString("first");
    cJSON_AddItemToArray(list, first);
    cJSON *second = cJSON_CreateString("second");
    cJSON_AddItemToArray(list, second);
    cJSON *num = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(list, num);
    cJSON_AddBoolToObject(root, "active", 1);

    // step 3: Operate and Validate
    double new_number = cJSON_SetNumberHelper(num, 42.0);
    cJSON *replacement = cJSON_CreateString("replaced_second");
    cJSON_ReplaceItemInArray(list, 1, replacement);
    char *repl_str = cJSON_GetStringValue(cJSON_GetArrayItem(list, 1));
    char *first_str = cJSON_GetStringValue(cJSON_GetArrayItem(list, 0));
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "original_first", first_str);
    cJSON_AddStringToObject(child, "current_second", repl_str);
    cJSON_AddNumberToObject(child, "updated_value", new_number);
    cJSON_AddItemToObject(root, "child", child);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}