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
//<ID> 1425
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(1.5);
    cJSON *num2 = cJSON_CreateNumber(2.5);
    cJSON *templ = cJSON_CreateObject();
    cJSON_AddNumberToObject(templ, "k", 7.0);

    // step 2: Configure
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToObject(root, "arr", array);
    cJSON_AddItemToObject(root, "template", templ);
    cJSON_AddNumberToObject(root, "initial", 10.0);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(array, 0);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    double v1 = cJSON_GetNumberValue(first);
    double v2 = cJSON_GetNumberValue(second);
    cJSON *tmpl = cJSON_GetObjectItem(root, "template");
    cJSON *tmpl_dup = cJSON_Duplicate(tmpl, 1);
    cJSON_AddItemToArray(array, tmpl_dup);
    cJSON_bool cmp = cJSON_Compare(tmpl, tmpl_dup, 1);
    cJSON_bool inval = cJSON_IsInvalid(first);
    cJSON *sum_item = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON_SetNumberHelper(sum_item, v1 + v2 + (double)cmp + (double)inval);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}