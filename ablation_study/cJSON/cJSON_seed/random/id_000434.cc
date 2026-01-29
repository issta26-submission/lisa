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
//<ID> 434
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
    cJSON *flag = (cJSON *)0;
    cJSON *got0 = (cJSON *)0;
    cJSON *doubled = (cJSON *)0;
    char *printed = (char *)0;
    double val = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(7.0);
    n1 = cJSON_CreateNumber(2.5);
    flag = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 4: Operate
    got0 = cJSON_GetArrayItem(arr, 0);
    val = cJSON_GetNumberValue(got0);
    doubled = cJSON_CreateNumber(val * 2.0);
    cJSON_AddItemToObject(root, "doubled_first", doubled);

    // step 5: Validate
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    cJSON_free((void *)printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)got0;
    (void)val;
    return 66;
}