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
//<ID> 422
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
    cJSON *got0 = (cJSON *)0;
    cJSON *got1 = (cJSON *)0;
    cJSON *sum_item = (cJSON *)0;
    char *printed = (char *)0;
    double v0 = 0.0;
    double v1 = 0.0;
    double sum = 0.0;
    cJSON_bool added_cs = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(100.5);
    n1 = cJSON_CreateNumber(200.25);

    // step 3: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    added_cs = cJSON_AddItemToObjectCS(root, "DataArray", arr);
    cJSON_AddTrueToObject(root, "isActive");
    cJSON_AddFalseToObject(root, "isDeprecated");

    // step 4: Operate
    got0 = cJSON_GetArrayItem(arr, 0);
    got1 = cJSON_GetArrayItem(arr, 1);
    v0 = cJSON_GetNumberValue(got0);
    v1 = cJSON_GetNumberValue(got1);
    sum = v0 + v1;
    sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObjectCS(root, "sum", sum_item);
    printed = cJSON_PrintUnformatted((const cJSON *)root);

    // step 5: Validate
    (void)added_cs;
    (void)printed;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}