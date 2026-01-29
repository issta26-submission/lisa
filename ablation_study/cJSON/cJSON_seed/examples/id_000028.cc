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
//<ID> 28
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *num_pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(child, "pi", num_pi);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(child, "maybe", maybe_null);
    double *dbl_array = (double *)cJSON_malloc(2 * sizeof(double));
    dbl_array[0] = 1.5;
    dbl_array[1] = 2.5;
    cJSON *darr = cJSON_CreateDoubleArray(dbl_array, 2);
    cJSON_AddItemToObject(child, "doubles", darr);

    // step 3: Operate & Validate
    cJSON *fetched_pi = cJSON_GetObjectItem(child, "pi");
    double pi_value = cJSON_GetNumberValue(fetched_pi);
    cJSON *sum_item = cJSON_CreateNumber(pi_value + dbl_array[0]);
    cJSON_AddItemToObject(child, "pi_plus_first", sum_item);

    // step 4: Cleanup
    cJSON_free(dbl_array);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}