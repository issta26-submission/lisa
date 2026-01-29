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
//<ID> 418
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
    cJSON *replacement = (cJSON *)0;
    cJSON *got = (cJSON *)0;
    char *printed = (char *)0;
    double extracted = 0.0;
    cJSON_bool replaced = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(1.5);
    n1 = cJSON_CreateNumber(2.5);

    // step 3: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddStringToObject(root, "note", "initial");

    // step 4: Operate
    replacement = cJSON_CreateNumber(9.75);
    replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);
    got = cJSON_GetArrayItem(arr, 1);
    extracted = cJSON_GetNumberValue(got);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    (void)replaced;
    (void)extracted;
    cJSON_Minify(printed);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}