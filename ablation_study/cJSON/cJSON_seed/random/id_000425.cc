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
//<ID> 425
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
    cJSON *objChild = (cJSON *)0;
    cJSON *innerNum = (cJSON *)0;
    cJSON *got0 = (cJSON *)0;
    cJSON *doubled = (cJSON *)0;
    char *printed = (char *)0;
    double value0 = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(42.0);
    n1 = cJSON_CreateString("example");
    objChild = cJSON_CreateObject();
    innerNum = cJSON_CreateNumber(7.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObjectCS(root, "caseKey", arr);
    cJSON_AddTrueToObject(root, "flag_true");
    cJSON_AddFalseToObject(root, "flag_false");
    cJSON_AddItemToObjectCS(objChild, "inner", innerNum);
    cJSON_AddItemToObjectCS(root, "child", objChild);

    // step 4: Operate
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    got0 = cJSON_GetArrayItem(arr, 0);
    value0 = cJSON_GetNumberValue(got0);

    // step 5: Validate
    doubled = cJSON_CreateNumber(value0 * 2.0);
    cJSON_AddItemToObjectCS(root, "doubled", doubled);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}