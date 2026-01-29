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
//<ID> 598
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double initial_vals[3] = { 1.0, 2.0, 3.0 };
    cJSON *orig_array = cJSON_CreateDoubleArray(initial_vals, 3);
    cJSON_AddItemToObject(root, "numbers", orig_array);
    cJSON *threshold = cJSON_CreateNumber(10.5);
    cJSON_AddItemToObject(root, "threshold", threshold);

    // step 2: Configure
    cJSON *numbers_item = cJSON_GetObjectItem(root, "numbers");
    cJSON *threshold_item = cJSON_GetObjectItem(root, "threshold");

    // step 3: Operate
    double thr_val = cJSON_GetNumberValue(threshold_item);
    double scaled_vals[3] = { thr_val, thr_val + 1.0, thr_val + 2.0 };
    cJSON *scaled_array = cJSON_CreateDoubleArray(scaled_vals, 3);
    cJSON_ReplaceItemViaPointer(root, numbers_item, scaled_array);

    // step 4: Validate & Cleanup
    cJSON *numbers_after = cJSON_GetObjectItem(root, "numbers");
    cJSON *first_elem = cJSON_GetArrayItem(numbers_after, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    (void)thr_val;
    (void)first_val;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}