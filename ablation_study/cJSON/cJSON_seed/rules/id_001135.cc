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
//<ID> 1135
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
    cJSON *elem0 = NULL;
    cJSON *elem1 = NULL;
    cJSON *elem_ref = NULL;
    cJSON *replacement = NULL;
    cJSON *title_orig = NULL;
    cJSON *title_new = NULL;
    const char *arr0_val = NULL;
    const char *arr1_val = NULL;
    const char *title_val = NULL;
    cJSON_bool ok_add_ref = 0;
    cJSON_bool ok_replace_arr = 0;
    cJSON_bool ok_replace_obj = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    elem0 = cJSON_CreateString("alpha");
    elem1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, elem0);
    cJSON_AddItemToArray(arr, elem1);
    title_orig = cJSON_CreateString("original");
    cJSON_AddItemToObject(root, "title", title_orig);

    // step 3: Operate / Validate
    elem_ref = cJSON_GetArrayItem(arr, 0);
    ok_add_ref = cJSON_AddItemReferenceToArray(arr, elem_ref);
    replacement = cJSON_CreateString("gamma");
    ok_replace_arr = cJSON_ReplaceItemInArray(arr, 1, replacement);
    title_new = cJSON_CreateString("replaced_title");
    ok_replace_obj = cJSON_ReplaceItemInObjectCaseSensitive(root, "title", title_new);
    arr0_val = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    arr1_val = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 1));
    title_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "title"));

    // step 4: Cleanup
    (void)ok_add_ref;
    (void)ok_replace_arr;
    (void)ok_replace_obj;
    (void)arr0_val;
    (void)arr1_val;
    (void)title_val;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}