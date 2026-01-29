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
//<ID> 1567
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
    cJSON *item_str = NULL;
    cJSON *item_true = NULL;
    cJSON *first = NULL;
    cJSON *copied = NULL;
    const char *first_val = NULL;
    char *unformatted = NULL;
    cJSON_bool is_str = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    item_str = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, item_str);
    item_true = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, item_true);

    // step 3: Operate / Validate
    first = cJSON_GetArrayItem(arr, 0);
    is_str = cJSON_IsString(first);
    first_val = cJSON_GetStringValue(first);
    copied = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "copied_first", copied);
    unformatted = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}