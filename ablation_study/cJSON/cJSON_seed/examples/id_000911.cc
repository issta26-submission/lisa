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
//<ID> 911
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON *str2 = cJSON_CreateString("alpha");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num_item = cJSON_CreateNumber(1.0);

    // step 2: Configure
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, true_item);
    double newnum = cJSON_SetNumberHelper(num_item, 42.5);

    // step 3: Operate & Validate
    cJSON_bool equal = cJSON_Compare(str1, str2, 1);
    cJSON_AddBoolToObject(root, "strings_equal", equal);
    cJSON *retrieved = cJSON_GetArrayItem(arr, 1);
    double retrieved_val = cJSON_GetNumberValue(retrieved);
    cJSON *computed = cJSON_CreateNumber(retrieved_val + newnum);
    cJSON_AddItemToArray(arr, computed);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}