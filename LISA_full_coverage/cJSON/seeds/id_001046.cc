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
//<ID> 1046
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nums = cJSON_CreateArray();
    cJSON_AddItemToArray(nums, cJSON_CreateNumber(10));
    cJSON_AddItemToArray(nums, cJSON_CreateNumber(20));
    cJSON_AddItemToArray(nums, cJSON_CreateNumber(30));
    cJSON_AddItemToObjectCS(root, "nums", nums);
    cJSON *name = cJSON_CreateString("tester");
    cJSON_AddItemToObjectCS(root, "name", name);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObjectCS(root, "active", flag);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON_SetValuestring(name_item, "tester_updated");
    cJSON_bool nums_is_array = cJSON_IsArray(cJSON_GetObjectItem(root, "nums"));
    cJSON_AddItemToObjectCS(root, "nums_is_array", cJSON_CreateBool(nums_is_array));

    // step 3: Operate
    cJSON *out = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(out, "copied_name", cJSON_CreateString(cJSON_GetStringValue(name_item)));
    cJSON *first = cJSON_GetArrayItem(nums, 0);
    cJSON *second = cJSON_GetArrayItem(nums, 1);
    double sum = cJSON_GetNumberValue(first) + cJSON_GetNumberValue(second);
    cJSON_AddItemToObjectCS(out, "sum_first_two", cJSON_CreateNumber(sum));
    char *pretty = cJSON_Print(out);
    char *compact = cJSON_PrintUnformatted(out);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_Delete(root);
    cJSON_Delete(out);

    // API sequence test completed successfully
    return 66;
}