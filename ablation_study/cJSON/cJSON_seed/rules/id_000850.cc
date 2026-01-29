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
//<ID> 850
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
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *detached = NULL;
    cJSON *expected_arr = NULL;
    char *printed = NULL;
    cJSON_bool has_arr = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "numbers");
    num1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num1);
    num2 = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, num2);

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromArray(arr, 1);
    expected_arr = cJSON_CreateArray();
    cJSON_AddItemToArray(expected_arr, cJSON_CreateNumber(42.0));
    equal = cJSON_Compare((const cJSON *const)arr, (const cJSON *const)expected_arr, 1);
    has_arr = cJSON_HasObjectItem(root, "numbers");
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(expected_arr);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    (void)equal;
    (void)has_arr;

    // API sequence test completed successfully
    return 66;
}