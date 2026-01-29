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
//<ID> 736
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"items\":[\"a\",\"b\"],\"count\":2}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    const char *strs[] = { "alpha", "beta", "gamma" };
    cJSON *new_str_array = cJSON_CreateStringArray(strs, 3);
    cJSON_InsertItemInArray(items, 1, new_str_array);
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double current_count = cJSON_GetNumberValue(count_item);
    cJSON_SetNumberHelper(count_item, current_count + 3.0);

    // step 3: Operate
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Validate & Cleanup
    int final_size = cJSON_GetArraySize(items);
    cJSON_AddItemToObject(root, "final_size", cJSON_CreateNumber((double)final_size));
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}