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
//<ID> 853
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *array = NULL;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *dup_root = NULL;
    cJSON *detached = NULL;
    cJSON *other_root = NULL;
    char *printed_root = NULL;
    char *printed_other = NULL;
    cJSON_bool has_arr = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    array = cJSON_AddArrayToObject(root, "arr");
    item0 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(array, item0);
    item1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(array, item1);
    dup_root = cJSON_Duplicate(root, 1);

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromArray(array, 0);
    other_root = cJSON_CreateObject();
    cJSON_AddItemToObject(other_root, "moved", detached);
    has_arr = cJSON_HasObjectItem(root, "arr");
    equal = cJSON_Compare(root, dup_root, 1);
    printed_root = cJSON_PrintUnformatted(root);
    printed_other = cJSON_PrintUnformatted(other_root);

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_other);
    cJSON_Delete(dup_root);
    cJSON_Delete(other_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)has_arr;
    (void)equal;
    return 66;
}