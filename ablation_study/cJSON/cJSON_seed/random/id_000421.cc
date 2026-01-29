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
//<ID> 421
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
    cJSON *name = (cJSON *)0;
    cJSON *got0 = (cJSON *)0;
    cJSON *doubled = (cJSON *)0;
    char *printed = (char *)0;
    double extracted = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(1.5);
    n1 = cJSON_CreateNumber(2.5);
    name = cJSON_CreateString("demo-name");

    // step 3: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObjectCS(root, "Name", name);
    cJSON_AddTrueToObject(root, "active");
    cJSON_AddFalseToObject(root, "deleted");

    // step 4: Operate
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    got0 = cJSON_GetArrayItem(arr, 0);
    extracted = cJSON_GetNumberValue(got0);

    // step 5: Validate
    doubled = cJSON_CreateNumber(extracted * 2.0);
    cJSON_AddItemToObjectCS(root, "DoubleValue", doubled);
    (void)extracted;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}