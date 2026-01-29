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
//<ID> 1134
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
    cJSON *item3 = NULL;
    cJSON *ref_item = NULL;
    cJSON *repl = NULL;
    cJSON *title = NULL;
    cJSON *new_title = NULL;
    cJSON *copy_item = NULL;
    const char *ref_value = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item1 = cJSON_CreateString("alpha");
    item2 = cJSON_CreateString("beta");
    item3 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    cJSON_AddItemToArray(arr, item3);
    cJSON_AddItemToObject(root, "items", arr);
    title = cJSON_CreateString("original");
    cJSON_AddItemToObject(root, "title", title);

    // step 3: Operate / Validate
    ref_item = cJSON_GetArrayItem(arr, 1);
    ref_value = cJSON_GetStringValue(ref_item);
    cJSON_AddItemReferenceToArray(arr, ref_item);
    repl = cJSON_CreateString("delta");
    cJSON_ReplaceItemInArray(arr, 0, repl);
    new_title = cJSON_CreateString("updated");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "title", new_title);
    copy_item = cJSON_CreateString(ref_value);
    cJSON_AddItemToObject(root, "copied", copy_item);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}