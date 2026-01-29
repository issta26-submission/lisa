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
//<ID> 1643
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *arr_ref = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *num3 = NULL;
    cJSON *got_arr = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    num1 = cJSON_CreateNumber(1.5);
    num2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    arr_ref = cJSON_CreateArrayReference(arr);
    cJSON_AddItemToObject(root, "arr_ref", arr_ref);

    // step 3: Operate / Validate
    got_arr = cJSON_GetObjectItem(root, "arr");
    num3 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(got_arr, num3);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}