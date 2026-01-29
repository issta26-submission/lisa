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
//<ID> 1133
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
    cJSON *item_a = NULL;
    cJSON *item_b = NULL;
    cJSON *first_item = NULL;
    cJSON *refs = NULL;
    cJSON *meta_item = NULL;
    cJSON *new_meta = NULL;
    cJSON *replacement_in_array = NULL;
    char *dynamic_str = NULL;

    dynamic_str = (char *)cJSON_malloc(5);
    memset(dynamic_str, 0, 5);
    dynamic_str[0] = 'b';
    dynamic_str[1] = 'e';
    dynamic_str[2] = 't';
    dynamic_str[3] = 'a';
    dynamic_str[4] = '\0';

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    item_a = cJSON_CreateString("alpha");
    item_b = cJSON_CreateString(dynamic_str);
    cJSON_AddItemToArray(arr, item_a);
    cJSON_AddItemToArray(arr, item_b);
    meta_item = cJSON_CreateString("original_meta");
    cJSON_AddItemToObject(root, "meta", meta_item);

    // step 3: Operate / Validate
    first_item = cJSON_GetArrayItem(arr, 0);
    refs = cJSON_CreateArray();
    cJSON_AddItemReferenceToArray(refs, first_item);
    cJSON_AddItemToObject(root, "refs", refs);
    replacement_in_array = cJSON_CreateString("replacement_value");
    cJSON_ReplaceItemInArray(arr, 1, replacement_in_array);
    new_meta = cJSON_CreateString("updated_meta");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "meta", new_meta);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(dynamic_str);
    // API sequence test completed successfully
    return 66;
}