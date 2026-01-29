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
//<ID> 379
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("example");
    cJSON *num = cJSON_CreateNumber(3.14);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "value", num);
    cJSON_AddItemToObject(root, "optional", maybe_null);

    // step 2: Configure
    cJSON *name_dup = cJSON_Duplicate(name, 0);
    cJSON_AddItemToObject(root, "name_copy", name_dup);
    double newnum = cJSON_SetNumberHelper(num, 42.0);
    cJSON *root_dup = cJSON_Duplicate(root, 1);

    // step 3: Operate & Validate
    double val = cJSON_GetNumberValue(num);
    const char *name_val = cJSON_GetStringValue(name_dup);
    cJSON_bool was_null = cJSON_IsNull(maybe_null);
    cJSON_bool equal = cJSON_Compare(root, root_dup, 1);
    char status_buf[64];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "%d|%d|%s", (int)was_null, (int)equal, name_val);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status);
    cJSON *sum = cJSON_CreateNumber(newnum + val);
    cJSON_AddItemToObject(root, "sum", sum);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}