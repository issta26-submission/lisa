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
//<ID> 71
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
    cJSON *num2 = cJSON_CreateNumber(2.71);
    cJSON_AddItemToArray(values, num1);
    cJSON_AddItemToArray(values, num2);
    char json_with_space[] = " { \"note\" : \" hello world \" , \"count\" :  7 } ";
    cJSON_Minify(json_with_space);
    cJSON *minified_str = cJSON_CreateString(json_with_space);
    cJSON_AddItemToObject(root, "minified", minified_str);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(values, 0);
    cJSON *second = cJSON_GetArrayItem(values, 1);
    double a = cJSON_GetNumberValue(first);
    double b = cJSON_GetNumberValue(second);
    cJSON *sum = cJSON_CreateNumber(a + b);
    cJSON_AddItemToObject(root, "sum", sum);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}