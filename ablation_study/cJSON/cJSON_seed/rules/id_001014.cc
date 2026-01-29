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
//<ID> 1014
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *values = NULL;
    cJSON *raw_item = NULL;
    cJSON *replacement = NULL;
    cJSON *flag_item = NULL;
    cJSON *first = NULL;
    cJSON_bool replaced = 0;
    cJSON_bool flag_is_true = 0;
    double first_val = 0.0;
    double nums[3] = {1.5, 2.5, 3.75};

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    values = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "values", values);
    raw_item = cJSON_CreateRaw("{\"rawKey\":123}");
    cJSON_AddItemToObject(root, "raw", raw_item);
    flag_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag_item);

    // step 3: Operate / Validate
    replacement = cJSON_CreateString("replaced_raw");
    replaced = cJSON_ReplaceItemViaPointer(root, raw_item, replacement);
    flag_is_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "flag"));
    first = cJSON_GetArrayItem(values, 0);
    first_val = cJSON_GetNumberValue(first);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}