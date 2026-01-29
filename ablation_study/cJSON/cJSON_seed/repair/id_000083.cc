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
//<ID> 83
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
    cJSON *n2 = cJSON_CreateNumber(2.718);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);

    // step 2: Configure
    cJSON *backup = cJSON_Duplicate(values, 1);
    cJSON_AddItemToObject(root, "backup", backup);
    cJSON *n3 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(values, n3);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(values);
    cJSON *first = cJSON_GetArrayItem(values, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_AddNumberToObject(root, "first_value", first_val);
    cJSON_AddNumberToObject(root, "count", (double)count);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}