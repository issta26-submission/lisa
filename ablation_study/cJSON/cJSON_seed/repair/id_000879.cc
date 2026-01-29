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
//<ID> 879
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(metrics, "values", values);
    cJSON *n0 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(values, n0);
    cJSON *n1 = cJSON_CreateNumber(2.71);
    cJSON_AddItemToArray(values, n1);
    cJSON *initial = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(metrics, "initial", initial);

    // step 2: Configure
    double new_initial = cJSON_SetNumberHelper(initial, 42.0);
    cJSON *scaled = cJSON_CreateNumber(new_initial * 2.0);
    cJSON_AddItemToObject(metrics, "scaled", scaled);

    // step 3: Operate and Validate
    cJSON *metrics_ref = cJSON_GetObjectItem(root, "metrics");
    cJSON *values_ref = cJSON_GetObjectItem(metrics_ref, "values");
    cJSON *first_item = cJSON_GetArrayItem(values_ref, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *copied_first = cJSON_CreateNumber(first_val);
    cJSON_AddItemToObject(root, "copied_first", copied_first);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}