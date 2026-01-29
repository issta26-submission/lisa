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
//<ID> 324
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

    // step 2: Configure
    cJSON *measurement = cJSON_CreateNumber(12.34);
    cJSON_AddItemToObject(metrics, "measurement", measurement);
    cJSON_AddBoolToObject(metrics, "measurement_positive", (cJSON_bool)(12.34 > 0.0));
    cJSON *got_measure = cJSON_GetObjectItem(metrics, "measurement");
    double val = cJSON_GetNumberValue(got_measure);

    // step 3: Operate and Validate
    cJSON *doubled = cJSON_CreateNumber(val * 2.0);
    cJSON_AddItemToObject(metrics, "measurement_double", doubled);
    cJSON *got_doubled = cJSON_GetObjectItem(metrics, "measurement_double");
    double val2 = cJSON_GetNumberValue(got_doubled);
    double sum = val + val2;
    char *out = cJSON_PrintUnformatted(root);
    (void)sum;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}