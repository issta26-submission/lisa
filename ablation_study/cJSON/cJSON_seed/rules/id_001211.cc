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
//<ID> 1211
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child_array = NULL;
    cJSON *greet = NULL;
    cJSON *elem1 = NULL;
    cJSON *elem2 = NULL;
    cJSON *greet_copy = NULL;
    char *printed = NULL;
    const char *greet_val = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child_array = cJSON_CreateArray();
    greet = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greet);
    cJSON_AddItemToObject(root, "list", child_array);
    elem1 = cJSON_CreateString("one");
    elem2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(child_array, elem1);
    cJSON_AddItemToArray(child_array, elem2);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(child_array);
    greet_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));
    greet_copy = cJSON_CreateString(greet_val);
    cJSON_AddItemToObject(root, "greeting_copy", greet_copy);
    printed = cJSON_Print(root);
    (void)arr_size;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}