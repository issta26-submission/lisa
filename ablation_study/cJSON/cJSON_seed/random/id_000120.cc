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
//<ID> 120
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *name_str = cJSON_CreateString("sensor_A");
    cJSON *status_str = (cJSON *)0;
    float measurements_vals[3] = { 1.1f, 2.2f, 3.3f };
    cJSON *measurements = cJSON_CreateFloatArray(measurements_vals, 3);
    cJSON *m0 = (cJSON *)0;
    cJSON *m1 = (cJSON *)0;
    cJSON *m2 = (cJSON *)0;
    double average = 0.0;
    char *json_text = (char *)0;

    // Step 2: Setup - attach name, status, measurements and an id
    cJSON_AddItemToObject(root, "name", name_str);
    status_str = cJSON_AddStringToObject(root, "status", "operational");
    cJSON_AddItemToObject(root, "measurements", measurements);
    cJSON_AddNumberToObject(root, "id", 1001.0);

    // Step 3: Core operations - read measurement items and compute average
    m0 = cJSON_GetArrayItem(measurements, 0);
    m1 = cJSON_GetArrayItem(measurements, 1);
    m2 = cJSON_GetArrayItem(measurements, 2);
    average = (cJSON_GetNumberValue(m0) + cJSON_GetNumberValue(m1) + cJSON_GetNumberValue(m2)) / 3.0;
    cJSON_AddNumberToObject(root, "average", average);

    // Step 4: Validate - produce and free JSON text
    json_text = cJSON_Print(root);
    cJSON_free(json_text);

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}