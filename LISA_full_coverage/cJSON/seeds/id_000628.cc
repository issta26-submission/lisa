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
//<ID> 628
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"items\":[10,20],\"meta\":{\"name\":\"example\"}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON_bool items_is_array = cJSON_IsArray(items);

    // step 2: Configure
    cJSON *new_arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(30.0);
    cJSON *n2 = cJSON_CreateNumber(40.0);
    cJSON_AddItemToArray(new_arr, n1);
    cJSON_AddItemToArray(new_arr, n2);
    cJSON_AddItemToObject(root, "new_items", new_arr);
    cJSON_bool newarr_is_array = cJSON_IsArray(new_arr);

    // step 3: Operate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    const char *parse_end_copy = NULL;
    cJSON *parsed_copy = cJSON_ParseWithOpts(buffer, &parse_end_copy, 1);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "items");
    cJSON_Delete(parsed_copy);
    cJSON_Delete(root);
    (void)items_is_array;
    (void)newarr_is_array;
    (void)printed;
    (void)parse_end;
    (void)parse_end_copy;
    // API sequence test completed successfully
    return 66;
}