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
//<ID> 1556
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
    cJSON *cmp_item = NULL;
    cJSON *replacement = NULL;
    cJSON *retrieved = NULL;
    void *dyn_name = NULL;
    int cmp_result = 0;
    int replace_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    item0 = cJSON_CreateString("first");
    item1 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cmp_item = cJSON_CreateString("first");
    dyn_name = cJSON_malloc(5); /* "note" + '\\0' */
    ((char *)dyn_name)[0] = 'n';
    ((char *)dyn_name)[1] = 'o';
    ((char *)dyn_name)[2] = 't';
    ((char *)dyn_name)[3] = 'e';
    ((char *)dyn_name)[4] = '\0';
    cJSON_AddStringToObject(root, (const char *)dyn_name, "added");
    cJSON_free(dyn_name);

    // step 3: Operate / Validate
    retrieved = cJSON_GetArrayItem(arr, 0);
    cmp_result = (int)cJSON_Compare(retrieved, cmp_item, 1);
    replacement = cJSON_CreateString("replaced");
    replace_result = (int)cJSON_ReplaceItemViaPointer(arr, retrieved, replacement);

    // step 4: Cleanup
    cJSON_Delete(cmp_item);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}