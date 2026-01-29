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
//<ID> 859
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
    cJSON *str_item = NULL;
    cJSON *num_item = NULL;
    cJSON *detached = NULL;
    cJSON *dup_root = NULL;
    char *printed = NULL;
    char *printed_dup = NULL;
    cJSON_bool has_arr = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "arr");
    str_item = cJSON_CreateString("first");
    cJSON_AddItemToArray(arr, str_item);
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num_item);

    // step 3: Operate / Validate
    dup_root = cJSON_Duplicate(root, 1);
    detached = cJSON_DetachItemFromArray(arr, 1);
    printed_dup = cJSON_PrintUnformatted(dup_root);
    printed = cJSON_PrintUnformatted(root);
    has_arr = cJSON_HasObjectItem(root, "arr");
    equal = cJSON_Compare(root, dup_root, 1);

    // step 4: Cleanup
    cJSON_free(printed_dup);
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)has_arr;
    (void)equal;
    return 66;
}