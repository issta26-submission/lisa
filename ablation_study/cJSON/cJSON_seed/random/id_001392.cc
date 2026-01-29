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
//<ID> 1392
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
    cJSON *picked = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *extracted_obj = (cJSON *)0;
    double picked_value = 0.0;
    int final_array_size = 0;

    // step 2: Initialize - create root object, array and numeric elements
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(42.0);
    n2 = cJSON_CreateNumber(7.5);

    // step 3: Configure - populate array and attach it to root
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate - get second array item, detach it from the array and add a new number to root
    picked = cJSON_GetArrayItem(arr, 1);
    picked_value = cJSON_GetNumberValue(picked);
    detached = cJSON_DetachItemViaPointer(arr, picked);
    cJSON_AddItemToObject(root, "moved", detached);
    cJSON_AddNumberToObject(root, "extracted", picked_value);

    // step 5: Validate - observe final array size and retrieve the newly added numeric object
    final_array_size = cJSON_GetArraySize(arr);
    extracted_obj = cJSON_GetObjectItem(root, "extracted");

    // step 6: Cleanup - free the entire cJSON tree
    cJSON_Delete(root);

    (void)final_array_size;
    (void)extracted_obj;
    (void)picked_value;
    (void)n1;
    (void)n2;
    (void)picked;
    (void)detached;

    // API sequence test completed successfully
    return 66;
}