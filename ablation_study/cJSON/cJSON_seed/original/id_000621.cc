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
//<ID> 621
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"nums\":[1,2],\"meta\":{\"scale\":2}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);

    // step 2: Configure
    cJSON *nums = cJSON_GetObjectItem(root, "nums");
    cJSON_bool nums_is_array = cJSON_IsArray(nums);
    cJSON *extra = cJSON_CreateArray();
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(extra, n3);
    cJSON_AddItemToObject(root, "extra", extra);

    // step 3: Operate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON_DeleteItemFromObject(root, "meta");

    // step 4: Validate & Cleanup
    cJSON *extra_ref = cJSON_GetObjectItem(root, "extra");
    cJSON_bool is_array_extra = cJSON_IsArray(extra_ref);
    double flags = (double)(nums_is_array + is_array_extra + printed);
    cJSON_AddNumberToObject(root, "flags", flags);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}