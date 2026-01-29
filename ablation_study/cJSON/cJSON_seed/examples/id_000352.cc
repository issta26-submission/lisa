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
//<ID> 352
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *const initial_colors[] = { "red", "green" };
    cJSON *colors = cJSON_CreateStringArray(initial_colors, 2);
    cJSON *extra_color = cJSON_CreateString("blue");
    cJSON_AddItemToArray(colors, extra_color);

    // step 2: Configure
    cJSON_AddItemToObject(root, "colors", colors);
    cJSON *value_num = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(value_num, 256.75);
    cJSON_AddItemToObject(root, "value", value_num);

    // step 3: Operate & Validate
    cJSON *replacement_color = cJSON_CreateString("yellow");
    cJSON_ReplaceItemInArray(colors, 0, replacement_color);
    int count = cJSON_GetArraySize(colors);
    cJSON *count_num = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "count", count_num);
    char *printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}