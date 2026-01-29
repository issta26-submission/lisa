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
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"title\":\"example\",\"count\":1}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *arr = cJSON_AddArrayToObject(root, "items");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, false_item);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool is_false = cJSON_IsFalse(first);
    cJSON *count = cJSON_GetObjectItem(root, "count");
    cJSON_SetNumberHelper(count, (double)size);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);

    return 66; // API sequence test completed successfully
}