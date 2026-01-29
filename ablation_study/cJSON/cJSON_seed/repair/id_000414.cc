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
//<ID> 414
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON *p = cJSON_Parse("3.5");
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, p);
    cJSON_AddItemToObject(root, "values", arr);

    // step 2: Configure
    double updated_n2 = cJSON_SetNumberHelper(n2, 20.5);

    // step 3: Operate
    cJSON *n3 = cJSON_CreateNumber(4.5);
    cJSON_AddItemToArray(arr, n3);
    double updated_n1 = cJSON_SetNumberHelper(n1, 10.0);

    // step 4: Validate and Cleanup
    double v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 2));
    double v3 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 3));
    double sum = updated_n1 + updated_n2 + v2 + v3;
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}