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
//<ID> 351
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *initial_strings[] = { "one", "two", "three" };
    cJSON *str_array = cJSON_CreateStringArray(initial_strings, 3);

    // step 2: Configure
    cJSON *extra_str = cJSON_CreateString("extra");
    cJSON_AddItemToArray(str_array, extra_str);
    cJSON_AddItemToObject(root, "strings", str_array);
    cJSON *number_item = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(number_item, 9.5);
    cJSON_AddItemToObject(root, "value", number_item);

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(str_array, 1, replacement);
    int count = cJSON_GetArraySize(str_array);
    double current_num = cJSON_GetNumberValue(number_item);
    double new_value = current_num + (double)count;
    cJSON_SetNumberHelper(number_item, new_value);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}