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
//<ID> 596
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
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *n3 = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    cJSON *retr_arr = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    cJSON *label_item = (cJSON *)0;
    int arr_size = 0;
    double first_value = 0.0;
    char *label_value = (char *)0;
    cJSON_bool added_ok1 = 0;
    cJSON_bool added_ok2 = 0;
    cJSON_bool added_ok3 = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(1.0);
    n2 = cJSON_CreateNumber(2.0);
    n3 = cJSON_CreateNumber(3.0);

    // step 3: Configure
    added_ok1 = cJSON_AddItemToArray(arr, n1);
    added_ok2 = cJSON_AddItemToArray(arr, n2);
    added_ok3 = cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddStringToObject(root, "label", "my_numbers");
    count_item = cJSON_AddNumberToObject(root, "count", 3.0);

    // step 4: Operate
    retr_arr = cJSON_GetObjectItem(root, "array");
    arr_size = cJSON_GetArraySize(retr_arr);
    first_item = cJSON_GetArrayItem(retr_arr, 0);
    first_value = cJSON_GetNumberValue(first_item);
    label_item = cJSON_GetObjectItem(root, "label");
    label_value = cJSON_GetStringValue(label_item);

    // step 5: Validate
    (void)added_ok1;
    (void)added_ok2;
    (void)added_ok3;
    (void)arr_size;
    (void)first_value;
    (void)label_value;
    (void)count_item;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}