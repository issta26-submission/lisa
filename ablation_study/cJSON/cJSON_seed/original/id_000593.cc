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
//<ID> 593
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *pi_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", pi_item);
    double initial_values[] = {1.1, 2.2, 3.3};
    cJSON *values_array = cJSON_CreateDoubleArray(initial_values, 3);
    cJSON_AddItemToObject(root, "values", values_array);

    // step 2: Configure
    cJSON *pi_lookup = cJSON_GetObjectItem(root, "pi");
    double pi_value = cJSON_GetNumberValue(pi_lookup);
    double replacement_values[] = {4.4, 5.5};
    cJSON *replacement_array = cJSON_CreateDoubleArray(replacement_values, 2);
    cJSON *old_values_ptr = cJSON_GetObjectItem(root, "values");

    // step 3: Operate
    cJSON_bool replace_ok = cJSON_ReplaceItemViaPointer(root, old_values_ptr, replacement_array);
    cJSON *values_after = cJSON_GetObjectItem(root, "values");
    cJSON *first_in_new = cJSON_GetArrayItem(values_after, 0);
    double first_value = cJSON_GetNumberValue(first_in_new);

    // step 4: Validate & Cleanup
    (void)pi_value;
    (void)first_value;
    (void)replace_ok;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}