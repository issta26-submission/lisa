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
//<ID> 730
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"items\":[1,2],\"label\":\"original\"}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON_SetNumberHelper(first_item, first_val + 42.0);
    const char *insert_strings[2] = { "alpha", "beta" };
    cJSON *string_array = cJSON_CreateStringArray(insert_strings, 2);

    // step 3: Operate
    cJSON_InsertItemInArray(items, 1, string_array);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Validate & Cleanup
    int final_count = cJSON_GetArraySize(items);
    cJSON_AddItemToObject(root, "final_count", cJSON_CreateNumber((double)final_count));
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}