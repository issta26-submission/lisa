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
//<ID> 409
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
    char *printed = (char *)0;
    double extracted = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(10.5);
    n1 = cJSON_CreateNumber(20.25);

    // step 3: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 4: Operate
    got0 = cJSON_GetArrayItem(arr, 0);
    extracted = cJSON_GetNumberValue(got0);
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    cJSON_Minify(printed);

    // step 5: Validate
    (void)extracted;
    cJSON_DeleteItemFromObjectCaseSensitive(root, "enabled");

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}