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
//<ID> 1725
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
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    cJSON *greet = (cJSON *)0;
    int arr_size = 0;
    char *greet_val = (char *)0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item1 = cJSON_CreateString("one");
    item2 = cJSON_CreateString("two");
    greet = cJSON_CreateString("hello");

    // step 3: Configure
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToObject(root, "greet", greet);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    greet_val = cJSON_GetStringValue(greet);

    // step 5: Validate
    validation = (int)(arr_size == 2) + (int)(greet_val != (char *)0 && greet_val[0] == 'h');
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)item1;
    (void)item2;
    (void)greet;
    (void)arr_size;
    (void)greet_val;
    // API sequence test completed successfully
    return 66;
}