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
//<ID> 1099
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
    cJSON *shared = NULL;
    cJSON *elem = NULL;
    cJSON *got = NULL;
    char *got_str = NULL;
    char *printed = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObjectCS(root, "list", arr);
    greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObjectCS(root, "greeting", greeting);
    shared = cJSON_CreateString("shared_value");
    cJSON_AddItemToObjectCS(root, "shared", shared);
    elem = cJSON_CreateString("first_elem");
    cJSON_AddItemToArray(arr, elem);
    cJSON_AddItemReferenceToArray(arr, shared);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    got = cJSON_GetArrayItem(arr, arr_size - 1);
    got_str = cJSON_GetStringValue(got);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}