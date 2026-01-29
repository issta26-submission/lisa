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
//<ID> 495
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"arr\":[1,2,3],\"flag\":true}";
    cJSON *root = cJSON_Parse(json);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *flag = cJSON_GetObjectItem(root, "flag");
    cJSON_bool arr_is_array = cJSON_IsArray(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);

    // step 2: Configure
    double sum_val = first_val + (double)arr_is_array;
    cJSON_AddNumberToObject(root, "sum", sum_val);
    cJSON_AddNullToObject(root, "maybe_null");
    cJSON_AddFalseToObject(root, "newfalse");

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    double retrieved_sum = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "sum"));
    (void)retrieved_sum;

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}