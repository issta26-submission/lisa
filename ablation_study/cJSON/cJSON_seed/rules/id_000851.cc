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
//<ID> 851
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
    cJSON *label = NULL;
    cJSON *detached = NULL;
    cJSON *dup_root = NULL;
    char *printed_original = NULL;
    char *printed_dup = NULL;
    cJSON_bool has_arr = 0;
    cJSON_bool equal_before_detach = 0;
    cJSON_bool equal_after_detach = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "numbers");
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    label = cJSON_CreateString("example_label");
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate / Validate
    dup_root = cJSON_Duplicate(root, 1);
    has_arr = cJSON_HasObjectItem(root, "numbers");
    printed_original = cJSON_PrintUnformatted(root);
    printed_dup = cJSON_PrintUnformatted(dup_root);
    equal_before_detach = cJSON_Compare(root, dup_root, 1);
    detached = cJSON_DetachItemFromArray(arr, 1);
    equal_after_detach = cJSON_Compare(root, dup_root, 1);

    // step 4: Cleanup
    cJSON_free(printed_original);
    cJSON_free(printed_dup);
    cJSON_Delete(detached);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)has_arr;
    (void)equal_before_detach;
    (void)equal_after_detach;
    return 66;
}