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
//<ID> 856
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
    cJSON *dup_root = NULL;
    char *printed = NULL;
    cJSON_bool has_values = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "values");
    num1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, num1);
    num2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, num2);

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromArray(arr, 0);
    dup_root = cJSON_Duplicate(root, 1);
    equal = cJSON_Compare(root, dup_root, 1);
    has_values = cJSON_HasObjectItem(root, "values");
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)has_values;
    (void)equal;
    return 66;
}