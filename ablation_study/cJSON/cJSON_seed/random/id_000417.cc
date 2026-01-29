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
//<ID> 417
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
    cJSON *repl = (cJSON *)0;
    cJSON *got_item = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    char *printed = (char *)0;
    double extracted = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(3.14);
    n1 = cJSON_CreateNumber(7.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddStringToObject(root, "status", "initialized");

    // step 4: Operate
    repl = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInArray(arr, 0, repl);

    // step 5: Validate
    got_item = cJSON_GetArrayItem(arr, 0);
    extracted = cJSON_GetNumberValue(got_item);
    n2 = cJSON_CreateNumber(extracted * 2.0);
    cJSON_AddItemToObject(root, "doubled", n2);
    printed = cJSON_PrintUnformatted(root);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}