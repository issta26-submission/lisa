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
//<ID> 623
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"items\":[1,2,3],\"name\":\"example\"}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON_bool items_is_array = cJSON_IsArray(items);
    int original_size = cJSON_GetArraySize(items);

    // step 2: Configure
    cJSON *more = cJSON_CreateArray();
    cJSON *n4 = cJSON_CreateNumber(4.0);
    cJSON *n5 = cJSON_CreateNumber(5.0);
    cJSON_AddItemToArray(more, n4);
    cJSON_AddItemToArray(more, n5);
    cJSON_AddItemToObject(root, "more", more);

    // step 3: Operate
    char buf[256];
    memset(buf, 0, sizeof(buf));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buf, (int)sizeof(buf), 1);
    const char *parse_end2 = NULL;
    cJSON *reparsed = cJSON_ParseWithOpts(buf, &parse_end2, 1);
    cJSON *re_items = cJSON_GetObjectItem(reparsed, "items");
    int re_size = cJSON_GetArraySize(re_items);
    cJSON *first = cJSON_GetArrayItem(re_items, 0);
    double first_val = cJSON_GetNumberValue(first);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "more");
    cJSON_Delete(reparsed);
    cJSON_Delete(root);
    (void)items_is_array;
    (void)original_size;
    (void)printed;
    (void)re_size;
    (void)first_val;
    // API sequence test completed successfully
    return 66;
}