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
//<ID> 1136
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *items = NULL;
    cJSON *refs = NULL;
    cJSON *it0 = NULL;
    cJSON *it1 = NULL;
    cJSON *it2 = NULL;
    cJSON *new_num = NULL;
    cJSON *title = NULL;
    cJSON *title_replacement = NULL;
    const cJSON *picked = NULL;
    double picked_value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    items = cJSON_CreateArray();
    it0 = cJSON_CreateString("first");
    it1 = cJSON_CreateNumber(7.0);
    it2 = cJSON_CreateString("third");
    cJSON_AddItemToArray(items, it0);
    cJSON_AddItemToArray(items, it1);
    cJSON_AddItemToArray(items, it2);
    cJSON_AddItemToObject(root, "items", items);
    title = cJSON_CreateString("initial title");
    cJSON_AddItemToObject(root, "title", title);
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 3: Operate / Validate
    picked = cJSON_GetArrayItem(items, 1);
    picked_value = cJSON_GetNumberValue(picked);
    new_num = cJSON_CreateNumber(picked_value + 100.0);
    cJSON_ReplaceItemInArray(items, 1, new_num);
    refs = cJSON_CreateArray();
    cJSON_AddItemReferenceToArray(refs, cJSON_GetArrayItem(items, 0));
    cJSON_AddItemToObject(root, "refs", refs);
    title_replacement = cJSON_CreateString("replaced title");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "title", title_replacement);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}