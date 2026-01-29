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
//<ID> 1393
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *n3 = (cJSON *)0;
    cJSON *found_arr = (cJSON *)0;
    cJSON *picked = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    double extracted_val = 0.0;
    int arr_size = 0;

    // step 2: Setup - create root object and populate an array of numbers
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(1.1);
    n2 = cJSON_CreateNumber(2.2);
    n3 = cJSON_CreateNumber(3.3);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 3: Operate - retrieve second array element, detach it from the array and add its value to root
    found_arr = cJSON_GetObjectItem(root, "numbers");
    picked = cJSON_GetArrayItem(found_arr, 1);
    detached = cJSON_DetachItemViaPointer(arr, picked);
    extracted_val = cJSON_GetNumberValue(detached);
    cJSON_AddNumberToObject(root, "extracted", extracted_val);

    // step 4: Validate - obtain the current array size after detachment
    arr_size = cJSON_GetArraySize(arr);
    (void)arr_size;

    // step 5: Cleanup - free the detached item and the whole tree
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}