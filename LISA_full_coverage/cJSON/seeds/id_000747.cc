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
//<ID> 747
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"meta\":{\"version\":1},\"items\":[{\"id\":1,\"name\":\"one\"},{\"id\":2,\"name\":\"two\"}],\"count\":2}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    int initial_size = cJSON_GetArraySize(items);
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    cJSON *dup_item = cJSON_Duplicate(second_item, 1);
    cJSON_AddItemToArray(items, dup_item);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "processed", flag);
    cJSON_AddNullToObject(root, "note");

    // step 3: Operate
    int final_size = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "final_size", (double)final_size);

    // step 4: Validate & Cleanup
    cJSON *shallow_copy = cJSON_Duplicate(root, 0);
    cJSON_Delete(shallow_copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}