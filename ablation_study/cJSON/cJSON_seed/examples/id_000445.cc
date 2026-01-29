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
//<ID> 445
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

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(values, num1);
    cJSON *num2 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(values, num2);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(values, flag_false);

    // step 3: Operate & Validate
    cJSON *retrieved_values = cJSON_GetObjectItem(root, "values");
    cJSON_bool retrieved_is_null = cJSON_IsNull(retrieved_values);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);
    const char *snap_text = cJSON_GetStringValue(cJSON_GetObjectItem(root, "snapshot"));
    char status_buf[192];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "count=%d|null=%d|snapshot=%s", cJSON_GetArraySize(retrieved_values), (int)retrieved_is_null, snap_text ? snap_text : "null");
    cJSON_AddStringToObject(root, "status", status_buf);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}