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
//<ID> 1049
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
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *meta = NULL;
    cJSON *got_item = NULL;
    cJSON *dup_root = NULL;
    char *printed = NULL;
    const char *extracted_str = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    item0 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, item0);
    item1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddStringToObject(root, "status", "active");
    meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "version", "1.2.3");

    // step 3: Operate / Validate
    got_item = cJSON_GetArrayItem(arr, 1);
    extracted_str = cJSON_GetStringValue(got_item);
    dup_root = cJSON_Duplicate(root, 1);
    cJSON_AddStringToObject(dup_root, "copied_from", extracted_str);
    printed = cJSON_Print(dup_root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}