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
//<ID> 420
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
    cJSON *sum_node = (cJSON *)0;
    cJSON *tptr = (cJSON *)0;
    cJSON *fptr = (cJSON *)0;
    cJSON *got = (cJSON *)0;
    char *printed = (char *)0;
    double v0 = 0.0;
    double v1 = 0.0;
    double sum = 0.0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added_obj = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(1.5);
    n1 = cJSON_CreateNumber(2.5);

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, n0);
    added1 = cJSON_AddItemToArray(arr, n1);
    added_obj = cJSON_AddItemToObjectCS(root, "numbers", arr);
    tptr = cJSON_AddTrueToObject(root, "enabled");
    fptr = cJSON_AddFalseToObject(root, "visible");

    // step 4: Operate
    got = cJSON_GetArrayItem(arr, 0);
    v0 = cJSON_GetNumberValue(got);
    got = cJSON_GetArrayItem(arr, 1);
    v1 = cJSON_GetNumberValue(got);
    sum = v0 + v1;
    sum_node = cJSON_CreateNumber(sum);
    cJSON_AddItemToObjectCS(root, "sum", sum_node);
    printed = cJSON_PrintUnformatted((const cJSON *)root);

    // step 5: Validate
    (void)added0;
    (void)added1;
    (void)added_obj;
    (void)tptr;
    (void)fptr;
    (void)printed;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}