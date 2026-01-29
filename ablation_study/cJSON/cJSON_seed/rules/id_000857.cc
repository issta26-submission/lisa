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
//<ID> 857
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
    cJSON *s_item = NULL;
    cJSON *n_item = NULL;
    cJSON *detached = NULL;
    cJSON *dup_root = NULL;
    char *printed = NULL;
    cJSON_bool has_item = 0;
    cJSON_bool equal = 0;
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "items");
    s_item = cJSON_CreateString("one");
    cJSON_AddItemToArray(arr, s_item);
    n_item = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, n_item);

    // step 3: Operate / Validate
    has_item = cJSON_HasObjectItem(root, "items");
    detached = cJSON_DetachItemFromArray(arr, 0);
    dup_root = cJSON_Duplicate(root, 1);
    equal = cJSON_Compare(root, dup_root, 1);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = (has_item ? '1' : '0');
    (void)buffer;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}