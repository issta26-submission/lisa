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
//<ID> 734
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"meta\":{\"count\":2},\"items\":[\"a\",\"b\"]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *count_item = cJSON_GetObjectItem(meta, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    double new_count = cJSON_SetNumberHelper(count_item, count_val + 1.0);
    const char *new_strings[] = { "x", "y", "z" };
    cJSON *str_arr = cJSON_CreateStringArray(new_strings, 3);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON_InsertItemInArray(items, 1, str_arr);

    // step 3: Operate
    char *printed = cJSON_Print(root);

    // step 4: Validate & Cleanup
    int items_size = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "items_size", (double)items_size);
    cJSON_AddNumberToObject(root, "new_count", new_count);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}