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
//<ID> 423
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *n0 = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *sum = (cJSON *)0;
    cJSON *got0 = (cJSON *)0;
    cJSON *got1 = (cJSON *)0;
    cJSON *sum_obj = (cJSON *)0;
    char *printed = (char *)0;
    double v0 = 0.0;
    double v1 = 0.0;
    double total = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(42.0);
    n1 = cJSON_CreateNumber(7.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObjectCS(root, "values", arr);
    cJSON_AddTrueToObject(root, "active");
    cJSON_AddFalseToObject(root, "deleted");

    // step 4: Operate
    got0 = cJSON_GetArrayItem(arr, 0);
    got1 = cJSON_GetArrayItem(arr, 1);
    v0 = cJSON_GetNumberValue(got0);
    v1 = cJSON_GetNumberValue(got1);
    total = v0 + v1;
    sum = cJSON_CreateNumber(total);
    cJSON_AddItemToObjectCS(root, "sum", sum);
    printed = cJSON_PrintUnformatted((const cJSON *)root);

    // step 5: Validate
    sum_obj = cJSON_GetObjectItem(root, "sum");
    (void)sum_obj;
    (void)v0;
    (void)v1;
    (void)total;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}