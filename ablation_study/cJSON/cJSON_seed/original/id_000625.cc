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
//<ID> 625
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"arr\":[1,2],\"to_remove\":\"x\"}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);

    // step 2: Configure
    cJSON *new_arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(3.0);
    cJSON *n2 = cJSON_CreateNumber(4.0);
    cJSON_AddItemToArray(new_arr, n1);
    cJSON_AddItemToArray(new_arr, n2);
    cJSON_AddItemToObject(root, "arr2", new_arr);
    cJSON *arr_ref = cJSON_GetObjectItem(root, "arr");
    cJSON_bool is_arr = cJSON_IsArray(arr_ref);

    // step 3: Operate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), is_arr);
    const char *parse_end2 = NULL;
    cJSON *roundtrip = cJSON_ParseWithOpts(buffer, &parse_end2, 1);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "to_remove");
    cJSON_Delete(roundtrip);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}