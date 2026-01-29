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
//<ID> 14
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"example\",\"values\":[3,7]}";
    cJSON *root = cJSON_Parse(json);
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON *injected = cJSON_AddArrayToObject(root, "injected");
    cJSON_AddItemToArray(injected, false_item);
    cJSON *orig_values = cJSON_GetObjectItem(root, "values");

    // step 3: Operate and Validate
    cJSON *first_val = cJSON_GetArrayItem(orig_values, 0);
    cJSON *dup_first = cJSON_Duplicate(first_val, 1);
    cJSON_AddItemToArray(injected, dup_first);
    double updated = cJSON_SetNumberHelper(dup_first, cJSON_GetNumberValue(dup_first) + 1.0);
    int injected_size = cJSON_GetArraySize(injected);
    cJSON_bool first_is_false = cJSON_IsFalse(cJSON_GetArrayItem(injected, 0));
    (void)updated;
    (void)injected_size;
    (void)first_is_false;

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}