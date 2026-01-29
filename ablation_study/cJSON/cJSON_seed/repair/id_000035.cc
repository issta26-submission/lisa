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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *mutable_str = cJSON_CreateString("old_value");

    // step 2: Configure
    cJSON_AddItemToArray(array, s1);
    cJSON_AddItemToArray(array, s2);
    cJSON_AddItemToArray(array, mutable_str);
    char *set_res = cJSON_SetValuestring(mutable_str, "new_value");

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(array);
    cJSON_AddNumberToObject(root, "count", (double)size);
    cJSON *third = cJSON_GetArrayItem(array, 2);
    const char *last_val = cJSON_GetStringValue(third);
    cJSON_AddStringToObject(root, "last", last_val);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    (void)set_res;
    return 66; // API sequence test completed successfully
}