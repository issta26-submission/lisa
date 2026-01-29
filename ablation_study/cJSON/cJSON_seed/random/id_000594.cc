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
//<ID> 594
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
    cJSON *arr_ptr = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *one_item = (cJSON *)0;
    cJSON *added_num1 = (cJSON *)0;
    cJSON *added_num2 = (cJSON *)0;
    int arr_size = 0;
    char *first_str = (char *)0;
    double one_val = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    added_num1 = cJSON_AddNumberToObject(root, "one", 1.5);
    added_num2 = cJSON_AddNumberToObject(root, "two", 2.5);
    cJSON_AddItemToArray(arr, cJSON_CreateString("first_item"));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(3.14159));
    cJSON_AddItemToObject(root, "arr", arr);

    // step 4: Operate
    arr_ptr = cJSON_GetObjectItem(root, "arr");
    arr_size = cJSON_GetArraySize(arr_ptr);
    first_item = cJSON_GetArrayItem(arr_ptr, 0);
    first_str = cJSON_GetStringValue(first_item);
    one_item = cJSON_GetObjectItem(root, "one");
    one_val = cJSON_GetNumberValue(one_item);

    // step 5: Validate
    (void)added_num1;
    (void)added_num2;
    (void)arr_size;
    (void)first_str;
    (void)one_val;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}