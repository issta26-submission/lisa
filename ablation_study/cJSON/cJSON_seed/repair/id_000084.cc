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
//<ID> 84
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
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(values, n1);
    cJSON *label = cJSON_CreateString("sample");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    cJSON *backup = cJSON_Duplicate(values, 1);
    cJSON_AddItemToObject(root, "backup_values", backup);
    cJSON *n2 = cJSON_CreateNumber(2.718);
    cJSON_AddItemToArray(backup, n2);

    // step 3: Operate and Validate
    cJSON *got_values = cJSON_GetObjectItem(root, "values");
    int count = cJSON_GetArraySize(got_values);
    cJSON_AddNumberToObject(root, "values_count", (double)count);
    cJSON *first = cJSON_GetArrayItem(got_values, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_AddNumberToObject(root, "first_value_snapshot", first_val);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}