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
//<ID> 1128
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    // step 1: Initialize
    const double nums[3] = {10.0, 20.5, 30.25};
    cJSON *root = cJSON_CreateObject();
    cJSON *dbl_arr = cJSON_CreateDoubleArray(nums, 3);

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", dbl_arr);
    cJSON *name_item = cJSON_CreateString("initial_name");
    cJSON_SetValuestring(name_item, "updated_name"); // do not free this pointer manually
    cJSON_AddItemToObject(root, "name", name_item);

    // step 3: Operate
    cJSON *item0 = cJSON_GetArrayItem(dbl_arr, 0);
    cJSON_bool is_num0 = cJSON_IsNumber(item0);
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(dbl_arr, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(dbl_arr, 1));
    double v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(dbl_arr, 2));
    double total = v0 + v1 + v2 + (double)is_num0;
    cJSON_AddNumberToObject(root, "total", total);

    // step 4: Validate & Cleanup
    char *unformatted = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(256);
    memset(prealloc, 0, 256);
    cJSON_PrintPreallocated(root, prealloc, 256, 1);
    cJSON_free(unformatted);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}