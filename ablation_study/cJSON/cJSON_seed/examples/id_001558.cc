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
//<ID> 1558
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
    cJSON_AddItemToArray(values, cJSON_CreateNumber(3.14));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(2.0));
    cJSON_AddItemToObject(root, "label", cJSON_CreateString("sample"));

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *printed = cJSON_Print(dup);

    // step 3: Operate & Validate
    cJSON *values_from_root = cJSON_GetObjectItem(root, "values");
    int size = cJSON_GetArraySize(values_from_root);
    double first = cJSON_GetNumberValue(cJSON_GetArrayItem(values_from_root, 0));
    double computed_value = first + (double)size;
    cJSON_AddItemToObject(dup, "computed", cJSON_CreateNumber(computed_value));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}