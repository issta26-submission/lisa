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
//<ID> 396
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[3] = {1.1, 2.2, 3.3};
    cJSON *array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "values", array);
    cJSON_AddItemToObject(root, "optional", maybe_null);
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 2: Configure
    cJSON *second_item = cJSON_GetArrayItem(array, 1);
    double orig_second = cJSON_GetNumberValue(second_item);
    double new_second = cJSON_SetNumberHelper(second_item, orig_second + 10.0);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "second_orig=%.2f|second_new=%.2f", orig_second, new_second);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status);

    // step 3: Operate & Validate
    cJSON_bool enabled = cJSON_IsTrue(flag);
    cJSON *enabled_report = cJSON_CreateBool(enabled);
    cJSON_AddItemToObject(root, "enabled_report", enabled_report);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_item = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_item);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}