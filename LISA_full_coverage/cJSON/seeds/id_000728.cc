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
//<ID> 728
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"active\":true,\"count\":3,\"items\":[\"alpha\",\"beta\",\"gamma\"]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    cJSON *items_item = cJSON_GetObjectItem(root, "items");

    // step 2: Configure
    cJSON *disabled_flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "disabled", disabled_flag);
    char scratch[512];
    memset(scratch, 0, sizeof(scratch));
    char *printed = cJSON_PrintBuffered(root, 256, 1);

    // step 3: Operate
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON *count_copy = cJSON_CreateNumber(count_val);
    cJSON_AddItemToObject(root, "count_copy", count_copy);
    cJSON *items_copy = cJSON_Duplicate(items_item, 1);
    cJSON_AddItemToObject(root, "items_copy", items_copy);
    cJSON *rechecked_active = cJSON_GetObjectItem(root, "active");

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}