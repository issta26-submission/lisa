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
//<ID> 127
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = cJSON_Parse("{\"replace_me\": 3.14, \"keep\":\"yes\"}");
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 2: Configure
    double nums[4] = {1.1, 2.2, 3.3, 4.4};
    cJSON *replacement = cJSON_CreateDoubleArray(nums, 4);
    cJSON *old_item = cJSON_GetObjectItem(parsed, "replace_me");
    cJSON_ReplaceItemViaPointer(parsed, old_item, replacement);
    cJSON *extra = cJSON_AddArrayToObject(root, "extra");
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(extra, n1);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(extra, n2);
    cJSON_AddStringToObject(root, "status", "ok");

    // step 3: Operate and Validate
    cJSON *got_parsed = cJSON_GetObjectItem(root, "parsed");
    cJSON *repl_arr = cJSON_GetObjectItem(got_parsed, "replace_me");
    cJSON *first = cJSON_GetArrayItem(repl_arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_AddNumberToObject(root, "first_value", first_val);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}