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
//<ID> 394
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[3];
    numbers[0] = 1.1;
    numbers[1] = 2.2;
    numbers[2] = 3.3;
    cJSON *values = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON *optional = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", optional);
    cJSON *scale = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(root, "scale", scale);

    // step 2: Configure
    cJSON *second_item = cJSON_GetArrayItem(values, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    double updated_scale = cJSON_SetNumberHelper(scale, second_value * 2.0);

    // step 3: Operate & Validate
    cJSON_bool is_flag_true = cJSON_IsTrue(flag);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "flag=%d|second=%.2f|scale=%.2f", (int)is_flag_true, second_value, updated_scale);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_item = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_item);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}