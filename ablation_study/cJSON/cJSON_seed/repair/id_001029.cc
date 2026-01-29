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
//<ID> 1029
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_AddObjectToObject(root, "metrics");
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(metrics, "values", values);

    // step 2: Configure
    cJSON_AddNumberToObject(metrics, "count", 3.0);
    cJSON_AddNumberToObject(metrics, "average", 2.5);
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);

    // step 3: Operate and Validate
    cJSON *got_metrics = cJSON_GetObjectItem(root, "metrics");
    cJSON *got_values = cJSON_GetObjectItem(got_metrics, "values");
    cJSON_bool is_array = cJSON_IsArray(got_values);
    cJSON *count_item = cJSON_GetObjectItem(got_metrics, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = (char)('0' + is_array);
    ((char *)scratch)[1] = (char)('A' + (int)count_val);
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}