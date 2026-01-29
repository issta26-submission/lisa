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
//<ID> 440
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *app_name = "fuzz_app";
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "app", cJSON_CreateString(app_name));
    cJSON_AddItemToObject(root, "numbers", numbers);

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(numbers, num1);
    cJSON_AddItemToArray(numbers, num2);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(numbers, flag_false);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(numbers, null_item);

    // step 3: Operate & Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "numbers");
    cJSON *first = cJSON_GetArrayItem(retrieved, 0);
    cJSON *second = cJSON_GetArrayItem(retrieved, 1);
    double v_first = cJSON_GetNumberValue(first);
    double v_second = cJSON_GetNumberValue(second);
    double sum = v_first + v_second;
    cJSON_bool is_null = cJSON_IsNull(null_item);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "sum=%.2f|null=%d", sum, (int)is_null);
    cJSON_AddStringToObject(root, "status", status_buf);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}