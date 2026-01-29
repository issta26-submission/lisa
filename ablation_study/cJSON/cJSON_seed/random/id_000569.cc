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
//<ID> 569
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
    cJSON *num_a = (cJSON *)0;
    cJSON *num_b = (cJSON *)0;
    cJSON *constant_item = (cJSON *)0;
    cJSON *retr = (cJSON *)0;
    char *printed = (char *)0;
    double retrieved_value = 0.0;
    int arr_size = 0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool add_ok4 = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_a = cJSON_CreateNumber(10.5);
    num_b = cJSON_CreateNumber(20.25);

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, num_a);
    add_ok2 = cJSON_AddItemToArray(arr, num_b);
    add_ok3 = cJSON_AddItemToArray(arr, cJSON_CreateNumber(30.75));
    add_ok4 = cJSON_AddItemToObject(root, "numbers", arr);
    constant_item = cJSON_AddNumberToObject(root, "constant", 99.0);

    // step 4: Operate
    retr = cJSON_GetObjectItem(root, "numbers");
    retrieved_value = cJSON_GetNumberValue(cJSON_GetArrayItem(retr, 1));
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)add_ok4;
    (void)constant_item;
    arr_size = cJSON_GetArraySize(retr);
    (void)arr_size;
    (void)retrieved_value;
    (void)printed;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}