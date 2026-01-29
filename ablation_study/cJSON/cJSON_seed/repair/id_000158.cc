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
//<ID> 158
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *num_a = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(values, num_a);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToArray(values, flag);
    cJSON *num_b = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(values, num_b);

    // step 3: Operate and Validate
    cJSON *elem0 = cJSON_GetArrayItem(values, 0);
    cJSON *elem2 = cJSON_GetArrayItem(values, 2);
    double v0 = cJSON_GetNumberValue(elem0);
    double v2 = cJSON_GetNumberValue(elem2);
    double total = v0 + v2;
    cJSON *total_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "total", total_item);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}