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
//<ID> 412
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
    cJSON *n_repl = (cJSON *)0;
    cJSON *got_item = (cJSON *)0;
    cJSON *str_node = (cJSON *)0;
    double extracted = 0.0;
    cJSON_bool replaced_ok = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(1.0);
    n1 = cJSON_CreateNumber(2.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate
    n_repl = cJSON_CreateNumber(9.5);
    replaced_ok = cJSON_ReplaceItemInArray(arr, 1, n_repl);
    str_node = cJSON_AddStringToObject(root, "status", "updated");
    got_item = cJSON_GetArrayItem(arr, 1);
    extracted = cJSON_GetNumberValue(got_item);

    // step 5: Validate
    (void)replaced_ok;
    (void)str_node;
    (void)extracted;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}