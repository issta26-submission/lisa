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
//<ID> 1729
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
    cJSON *str0 = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    char *extracted = (char *)0;
    int arr_size = 0;
    int validation = 0;
    cJSON_bool add_ok = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str0 = cJSON_CreateString("first");
    str1 = cJSON_CreateString("second");

    // step 3: Configure
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddItemToArray(arr, str1);
    add_ok = cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "label", cJSON_CreateString("example"));

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    extracted = cJSON_GetStringValue(str0);

    // step 5: Validate
    validation = (int)(arr_size == 2) + (int)(extracted != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)str0;
    (void)str1;
    (void)extracted;
    (void)arr_size;
    (void)add_ok;
    // API sequence test completed successfully
    return 66;
}