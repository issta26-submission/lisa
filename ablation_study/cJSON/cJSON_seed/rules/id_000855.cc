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
//<ID> 855
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
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON *detached = NULL;
    cJSON *dup_root = NULL;
    char *printed = NULL;
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool has_list = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "list");
    item1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, item1);
    item2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, item2);
    cJSON_AddStringToObject(root, "label", "root_label");
    has_list = cJSON_HasObjectItem(root, "list");

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON_AddItemToObject(root, "second", detached);
    dup_root = cJSON_Duplicate(root, 1);
    printed = cJSON_PrintUnformatted(root);
    equal = cJSON_Compare(root, dup_root, 1);
    buffer[0] = (char)('0' + (int)has_list);
    buffer[1] = (char)('0' + (int)equal);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    (void)buffer;
    return 66;
}