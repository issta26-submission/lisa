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
//<ID> 92
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(info, "name", name);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(values, n1);
    cJSON *n2 = cJSON_CreateNumber(32.0);
    cJSON_AddItemToArray(values, n2);

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(name, "updated_name");
    cJSON *dup_info = cJSON_Duplicate(info, 1);
    cJSON_AddItemToObject(root, "info_backup", dup_info);

    // step 3: Operate and Validate
    cJSON *got_name = cJSON_GetObjectItem(info, "name");
    cJSON *got_name_backup = cJSON_GetObjectItem(dup_info, "name");
    int count = cJSON_GetArraySize(values);
    cJSON *first = cJSON_GetArrayItem(values, 0);
    cJSON *second = cJSON_GetArrayItem(values, 1);
    double v0 = cJSON_GetNumberValue(first);
    double v1 = cJSON_GetNumberValue(second);
    double sum = v0 + v1;
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON_AddNumberToObject(root, "count", (double)count);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    (void)set_ret;
    (void)got_name;
    (void)got_name_backup;
    return 66;
    // API sequence test completed successfully
}