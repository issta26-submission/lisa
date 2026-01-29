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
//<ID> 416
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
    cJSON *got = (cJSON *)0;
    cJSON *copied = (cJSON *)0;
    double extracted = 0.0;
    cJSON_bool added_ok = 0;
    cJSON_bool replaced_ok = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(42.0);
    n1 = cJSON_CreateNumber(84.0);
    repl = cJSON_CreateNumber(100.5);

    // step 3: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    added_ok = cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddStringToObject(root, "status", "ok");

    // step 4: Operate
    replaced_ok = cJSON_ReplaceItemInArray(arr, 1, repl);

    // step 5: Validate
    got = cJSON_GetArrayItem(arr, 1);
    extracted = cJSON_GetNumberValue(got);
    copied = cJSON_CreateNumber(extracted + 1.0);
    cJSON_AddItemToObject(root, "extracted_plus_one", copied);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}