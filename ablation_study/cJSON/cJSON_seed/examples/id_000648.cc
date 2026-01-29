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
//<ID> 648
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *item_null = cJSON_CreateNull();
    cJSON *item_num = cJSON_CreateNumber(42.0);
    cJSON *item_str = cJSON_CreateString("omega");

    // step 2: Configure
    cJSON_AddItemToArray(array, item_null);
    cJSON_AddItemToArray(array, item_num);
    cJSON_AddItemToArray(array, item_str);
    cJSON_AddItemToObject(root, "items", array);

    // step 3: Operate & Validate
    char buffer1[256];
    memset(buffer1, 0, sizeof(buffer1));
    cJSON_PrintPreallocated(root, buffer1, (int)sizeof(buffer1), 1);
    cJSON *first_item = cJSON_GetArrayItem(array, 0);
    cJSON *replacement = cJSON_CreateString("replaced_null");
    cJSON_ReplaceItemViaPointer(array, first_item, replacement);
    char buffer2[256];
    memset(buffer2, 0, sizeof(buffer2));
    cJSON_PrintPreallocated(root, buffer2, (int)sizeof(buffer2), 0);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}