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
//<ID> 416
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *doc = cJSON_Parse("{\"values\": [10.0, 20.0, 30.0], \"meta\": {\"scale\": 2.5}}");
    cJSON *values = cJSON_GetObjectItem(doc, "values");
    cJSON *meta = cJSON_GetObjectItem(doc, "meta");
    cJSON *scale_item = cJSON_GetObjectItem(meta, "scale");
    double scale = cJSON_GetNumberValue(scale_item);

    // step 2: Configure
    cJSON *scaled_arr = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(0.0);
    cJSON *n1 = cJSON_CreateNumber(0.0);
    cJSON *n2 = cJSON_CreateNumber(0.0);
    double orig0 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 0));
    double orig1 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 1));
    double orig2 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 2));
    double set0 = cJSON_SetNumberHelper(n0, orig0 * scale);
    (void)set0;
    cJSON_SetNumberHelper(n1, orig1 * scale);
    cJSON_SetNumberHelper(n2, orig2 * scale);
    cJSON_AddItemToArray(scaled_arr, n0);
    cJSON_AddItemToArray(scaled_arr, n1);
    cJSON_AddItemToArray(scaled_arr, n2);

    // step 3: Operate
    cJSON_AddItemToObject(doc, "scaled", scaled_arr);
    cJSON *retrieved = cJSON_GetObjectItem(doc, "scaled");
    double s0 = cJSON_GetNumberValue(cJSON_GetArrayItem(retrieved, 0));
    double s1 = cJSON_GetNumberValue(cJSON_GetArrayItem(retrieved, 1));
    double s2 = cJSON_GetNumberValue(cJSON_GetArrayItem(retrieved, 2));
    double sum_scaled = s0 + s1 + s2;
    cJSON_AddNumberToObject(doc, "sum_scaled", sum_scaled);

    // step 4: Validate and Cleanup
    int count = cJSON_GetArraySize(retrieved);
    (void)count;
    cJSON_Delete(doc);
    // API sequence test completed successfully
    return 66;
}