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
//<ID> 1557
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
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(5.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddNumberToObject(root, "count", 3);

    // step 2: Configure
    char *printed = cJSON_Print(root);
    cJSON *dup = cJSON_Duplicate(root, 1);

    // step 3: Operate & Validate
    cJSON *dup_values = cJSON_GetObjectItem(dup, "values");
    int size = cJSON_GetArraySize(dup_values);
    cJSON *first = cJSON_GetArrayItem(dup_values, 0);
    double first_val = cJSON_GetNumberValue(first);
    double computed = first_val + (double)size;
    cJSON_AddNumberToObject(dup, "computed", computed);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}