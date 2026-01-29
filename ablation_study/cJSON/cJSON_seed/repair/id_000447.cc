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
//<ID> 447
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *s3 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON_AddItemToArray(items, s3);

    // step 2: Configure
    cJSON_AddStringToObject(root, "title", "SampleList");
    cJSON *title_item = cJSON_GetObjectItem(root, "title");
    cJSON_bool title_is_string = cJSON_IsString(title_item);
    cJSON *title_flag = cJSON_CreateBool(title_is_string);
    cJSON_AddItemToObject(root, "title_is_string", title_flag);

    // step 3: Operate
    cJSON *items_ref = cJSON_GetObjectItem(root, "items");
    cJSON_DeleteItemFromArray(items_ref, 1);

    // step 4: Validate and Cleanup
    cJSON *first_item = cJSON_GetArrayItem(items_ref, 0);
    cJSON_AddStringToObject(root, "first_remaining", cJSON_GetStringValue(first_item));
    char *printed = cJSON_Print(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}