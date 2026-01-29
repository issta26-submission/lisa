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
//<ID> 354
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *initial_colors[3] = { "red", "green", "blue" };
    cJSON *root = cJSON_CreateObject();
    cJSON *palette = cJSON_CreateStringArray(initial_colors, 3);
    cJSON *extra_color = cJSON_CreateString("yellow");
    cJSON_AddItemToArray(palette, extra_color);

    // step 2: Configure
    cJSON *replacement_number = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(replacement_number, 77.77);
    cJSON_ReplaceItemInArray(palette, 1, replacement_number);

    // step 3: Operate & Validate
    cJSON_AddItemToObject(root, "colors", palette);
    int count = cJSON_GetArraySize(palette);
    cJSON *count_item = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "count", count_item);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}