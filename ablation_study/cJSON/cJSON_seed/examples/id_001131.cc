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
//<ID> 1131
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    double measurements_vals[3] = {1.5, 2.5, 3.5};
    cJSON *measurements = cJSON_CreateDoubleArray(measurements_vals, 3);
    cJSON *status = cJSON_CreateTrue();
    cJSON *label = cJSON_CreateString("sensor-A");
    cJSON *identifier = cJSON_CreateNumber(1001.0);

    // step 2: Configure
    cJSON_AddItemToObject(data, "measurements", measurements);
    cJSON_AddItemToObjectCS(root, "data", data);
    cJSON_AddItemToObject(root, "status", status);
    cJSON_AddItemToObject(root, "label", label);
    cJSON_AddItemToObject(root, "id", identifier);

    // step 3: Operate & Validate
    cJSON_bool is_active = cJSON_IsTrue(status);
    cJSON *m0 = cJSON_GetArrayItem(measurements, 0);
    cJSON *m1 = cJSON_GetArrayItem(measurements, 1);
    double v0 = cJSON_GetNumberValue(m0);
    double v1 = cJSON_GetNumberValue(m1);
    double partial_sum = v0 + v1 + (double)is_active;
    cJSON_AddNumberToObject(root, "partial_sum", partial_sum);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}