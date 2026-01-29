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
//<ID> 157
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
    cJSON *n1 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(values, n1);
    cJSON *n2 = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(values, n2);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToArray(values, flag);

    // step 2: Configure
    cJSON *scale = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(root, "scale", scale);
    cJSON *note = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "note_placeholder", note);

    // step 3: Operate and Validate
    double v1 = cJSON_GetNumberValue(n1);
    double v2 = cJSON_GetNumberValue(n2);
    double s = v1 + v2;
    double scale_val = cJSON_GetNumberValue(scale);
    double scaled = s * scale_val;
    cJSON *scaled_num = cJSON_CreateNumber(scaled);
    cJSON_AddItemToObject(root, "scaled_sum", scaled_num);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}