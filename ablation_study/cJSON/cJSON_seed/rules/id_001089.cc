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
//<ID> 1089
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
    cJSON *greeting = NULL;
    cJSON *elem1 = NULL;
    cJSON *elem2 = NULL;
    cJSON *false_item = NULL;
    cJSON *got = NULL;
    char *printed = NULL;
    char *got_str = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "greeting", greeting);
    elem1 = cJSON_CreateString("first");
    elem2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddItemToArray(arr, elem2);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, false_item);

    // step 3: Operate / Validate
    got = cJSON_GetObjectItem(root, "greeting");
    got_str = cJSON_GetStringValue(got);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}