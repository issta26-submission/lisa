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
//<ID> 592
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
    cJSON *num = (cJSON *)0;
    cJSON *retr_arr = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *second_item = (cJSON *)0;
    char *first_str = (char *)0;
    char *second_str = (char *)0;
    int arr_size = 0;
    double added_val = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str0 = cJSON_CreateString("alpha");
    str1 = cJSON_CreateString("beta");
    num = cJSON_CreateNumber(7.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *added_number = cJSON_AddNumberToObject(root, "magic", 13.5);
    added_val = (added_number != (cJSON *)0) ? added_number->valuedouble : 0.0;

    // step 4: Operate
    retr_arr = cJSON_GetObjectItem(root, "arr");
    arr_size = cJSON_GetArraySize(retr_arr);
    first_item = cJSON_GetArrayItem(retr_arr, 0);
    second_item = cJSON_GetArrayItem(retr_arr, 1);
    first_str = cJSON_GetStringValue(first_item);
    second_str = cJSON_GetStringValue(second_item);

    // step 5: Validate
    (void)arr_size;
    (void)first_str;
    (void)second_str;
    (void)added_val;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}