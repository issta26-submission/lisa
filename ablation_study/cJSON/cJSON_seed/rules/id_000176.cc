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
//<ID> 176
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting_orig = cJSON_CreateString("hello");
    cJSON *arr = cJSON_CreateArray();
    cJSON *elem1 = cJSON_CreateString("first");
    cJSON *elem2 = cJSON_CreateString("second");
    cJSON *replacement = cJSON_CreateString("hi");
    cJSON_bool replaced = 0;
    cJSON_bool compared = 0;
    cJSON *got_greeting = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "greeting", greeting_orig);
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddItemToArray(arr, elem2);
    cJSON_AddItemToObject(root, "list", arr);

    // step 3: Operate / Validate
    replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "greeting", replacement);
    got_greeting = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    compared = cJSON_Compare((const cJSON *const)replacement, (const cJSON *const)got_greeting, 1);
    (void)replaced;
    (void)compared;
    (void)got_greeting;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}