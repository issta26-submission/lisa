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
//<ID> 780
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[3] = {1.5, 2.5, 3.5};
    cJSON *dbl_array = cJSON_CreateDoubleArray(nums, 3);
    cJSON *label = cJSON_CreateString("example");

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", dbl_array);
    cJSON_AddItemToObject(root, "label", label);
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON *duplicate = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, duplicate, 1);
    cJSON *first = cJSON_GetArrayItem(dbl_array, 0);
    double first_val = cJSON_GetNumberValue(first);
    (void)equal;
    (void)first_val;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(duplicate);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}