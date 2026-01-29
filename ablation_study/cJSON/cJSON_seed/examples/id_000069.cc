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
//<ID> 69
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
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(values, num1);
    cJSON_AddItemToArray(values, num2);
    cJSON *special = cJSON_CreateNumber(-7.5);
    cJSON_AddItemToObjectCS(root, "special", special);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(values, 0);
    cJSON *second = cJSON_GetArrayItem(values, 1);
    double v0 = cJSON_GetNumberValue(first);
    double v1 = cJSON_GetNumberValue(second);
    cJSON *sum = cJSON_CreateNumber(v0 + v1);
    cJSON_AddItemToObject(root, "sum", sum);
    cJSON_bool has_values = cJSON_HasObjectItem(root, "values");
    cJSON_bool has_special = cJSON_HasObjectItem(root, "special");
    cJSON *flag1 = cJSON_CreateNumber((double)has_values);
    cJSON *flag2 = cJSON_CreateNumber((double)has_special);
    cJSON *flags = cJSON_CreateArray();
    cJSON_AddItemToArray(flags, flag1);
    cJSON_AddItemToArray(flags, flag2);
    cJSON_AddItemToObjectCS(root, "flags", flags);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}