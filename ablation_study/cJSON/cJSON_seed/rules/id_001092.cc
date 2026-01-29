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
//<ID> 1092
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
    cJSON *shared = NULL;
    cJSON *greeting = NULL;
    cJSON *got_greeting = NULL;
    cJSON *arr_first = NULL;
    char *got_str = NULL;
    char *arr_first_str = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObjectCS(root, "items", arr);
    shared = cJSON_CreateString("shared_element");
    cJSON_AddItemToObjectCS(root, "shared", shared);
    cJSON_AddItemReferenceToArray(arr, shared);
    greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObjectCS(root, "greeting", greeting);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    got_greeting = cJSON_GetObjectItem(root, "greeting");
    got_str = cJSON_GetStringValue(got_greeting);
    arr_first = cJSON_GetArrayItem(arr, 0);
    arr_first_str = cJSON_GetStringValue(arr_first);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)arr_size;
    (void)got_str;
    (void)arr_first_str;
    return 66;
}