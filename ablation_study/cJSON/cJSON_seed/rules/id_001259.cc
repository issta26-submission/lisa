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
//<ID> 1259
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const double nums[3] = {1.5, 2.5, 3.5};
    cJSON *root = cJSON_CreateObject();
    cJSON *darr = cJSON_CreateDoubleArray(nums, 3);
    cJSON *arr_obj = NULL;
    cJSON *second = NULL;
    double second_val = 0.0;
    char *printed = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "numbers", darr);
    cJSON_AddNullToObject(root, "missing");
    cJSON_AddStringToObject(root, "status", "ok");
    cJSON_AddRawToObject(root, "raw_meta", "{\"raw\":true}");

    // step 3: Operate / Validate
    arr_obj = cJSON_GetObjectItem(root, "numbers");
    second = cJSON_GetArrayItem(arr_obj, 1);
    second_val = cJSON_GetNumberValue(second);
    printed = cJSON_PrintUnformatted(root);
    (void)second_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}