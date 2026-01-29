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
//<ID> 1450
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"metrics\":{\"values\":[1,2,3]},\"status\":\"ok\"}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *metrics = cJSON_GetObjectItem(root, "metrics");
    cJSON *values = cJSON_GetObjectItem(metrics, "values");

    // step 2: Configure
    cJSON *new_num = cJSON_CreateNumber(99.5);
    cJSON_AddItemToArray(values, new_num);
    cJSON *values_ref = cJSON_CreateArrayReference(values);
    cJSON_AddItemToObject(root, "values_ref", values_ref);

    // step 3: Operate & Validate
    cJSON_bool has_metrics = cJSON_HasObjectItem(root, "metrics");
    cJSON *first_item = cJSON_GetArrayItem(values, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    (void)has_metrics;
    (void)first_val;
    (void)printed_ok;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}