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
//<ID> 626
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"items\":[1,2],\"flag\":true}";
    const char *parse_end1 = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end1, 1);
    cJSON *new_array = cJSON_CreateArray();
    cJSON *a1 = cJSON_CreateNumber(10.0);
    cJSON *a2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(new_array, a1);
    cJSON_AddItemToArray(new_array, a2);
    cJSON_AddItemToObject(root, "new_items", new_array);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *first = cJSON_GetArrayItem(items, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_bool is_arr = cJSON_IsArray(items);
    double adjusted_val = first_val + (double)is_arr * 100.0;
    cJSON *adjusted = cJSON_CreateNumber(adjusted_val);
    cJSON_AddItemToObject(root, "adjusted", adjusted);

    // step 3: Operate
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    const char *parse_end2 = NULL;
    cJSON *reparsed = cJSON_ParseWithOpts(buffer, &parse_end2, 1);
    cJSON_DeleteItemFromObject(root, "flag");

    // step 4: Validate & Cleanup
    cJSON_Delete(reparsed);
    cJSON_Delete(root);
    (void)printed;
    (void)is_arr;
    (void)parse_end1;
    (void)parse_end2;
    // API sequence test completed successfully
    return 66;
}