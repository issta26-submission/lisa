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
//<ID> 598
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
    cJSON *numA = (cJSON *)0;
    cJSON *numB = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *got_arr = (cJSON *)0;
    cJSON *got_elem0 = (cJSON *)0;
    cJSON *got_elem2 = (cJSON *)0;
    cJSON *got_str_item = (cJSON *)0;
    int arr_size = 0;
    const char *salute = (const char *)0;
    double first_num_value = 0.0;
    (void)first_num_value;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    numA = cJSON_CreateNumber(7.0);
    numB = cJSON_CreateNumber(8.5);
    str_item = cJSON_CreateString("hello");

    // step 3: Configure
    cJSON_AddNumberToObject(root, "added_via_helper", 123.0);
    cJSON_AddItemToArray(arr, numA);
    cJSON_AddItemToArray(arr, numB);
    cJSON_AddItemToArray(arr, cJSON_CreateString("elem_string"));
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "salute", str_item);

    // step 4: Operate
    got_arr = cJSON_GetObjectItem(root, "array");
    arr_size = cJSON_GetArraySize(got_arr);
    got_elem0 = cJSON_GetArrayItem(got_arr, 0);
    got_elem2 = cJSON_GetArrayItem(got_arr, 2);
    got_str_item = cJSON_GetObjectItem(root, "salute");
    salute = cJSON_GetStringValue(got_str_item);
    first_num_value = 0.0;
    if (got_elem0) { first_num_value = cJSON_GetNumberValue(got_elem0); } // harmless straight-line assignment

    // step 5: Validate
    (void)arr_size;
    (void)got_elem2;
    (void)salute;
    (void)first_num_value;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}