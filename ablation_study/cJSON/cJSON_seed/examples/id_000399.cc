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
//<ID> 399
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double values[3] = {1.5, 2.5, 3.5};
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateDoubleArray(values, 3);
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe_null);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON *first_item = cJSON_GetArrayItem(numbers, 0);
    double old_val = cJSON_GetNumberValue(first_item);
    double doubled = cJSON_SetNumberHelper(first_item, old_val * 2.0);
    cJSON *first_doubled = cJSON_CreateNumber(doubled);
    cJSON_AddItemToObject(root, "first_doubled", first_doubled);

    // step 3: Operate & Validate
    cJSON_bool flag_is_true = cJSON_IsTrue(flag);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "flag=%d|old=%.2f|doubled=%.2f", (int)flag_is_true, old_val, doubled);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}