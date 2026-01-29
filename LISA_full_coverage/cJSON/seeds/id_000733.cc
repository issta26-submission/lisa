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
//<ID> 733
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"meta\":{\"count\":2},\"items\":[\"alpha\",\"beta\"]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *count_item = cJSON_GetObjectItem(meta, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON_SetNumberHelper(count_item, count_val + 1.0);

    // step 2: Configure
    const char *new_strings[] = {"gamma", "delta"};
    cJSON *new_str_array = cJSON_CreateStringArray(new_strings, 2);
    cJSON *items = cJSON_GetObjectItem(root, "items");

    // step 3: Operate
    cJSON_InsertItemInArray(items, 1, new_str_array);
    char *printed = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}