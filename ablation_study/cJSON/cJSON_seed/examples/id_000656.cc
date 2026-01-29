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
//<ID> 656
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double nums[3] = {1.1, 2.2, 3.3};
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    int size_before = cJSON_GetArraySize(values);
    cJSON_AddNumberToObject(root, "count", (double)size_before);
    cJSON *replacement = cJSON_CreateString("array_replaced");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "values", replacement);

    // step 3: Operate & Validate
    char *pretty = cJSON_Print(root);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%s", pretty);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_values = cJSON_GetObjectItem(parsed, "values");
    char *val_str = cJSON_GetStringValue(parsed_values);
    cJSON_AddStringToObject(root, "final", val_str);

    // step 4: Cleanup
    cJSON_free(pretty);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}