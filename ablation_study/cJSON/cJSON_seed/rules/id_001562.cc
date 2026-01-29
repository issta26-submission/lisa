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
//<ID> 1562
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
    cJSON *str_item = NULL;
    cJSON *true_item = NULL;
    cJSON *got_item = NULL;
    cJSON *copied_item = NULL;
    char *printed = NULL;
    char *copy = NULL;
    const char *str_val = NULL;
    size_t len = 0;
    cJSON_bool is_str = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array", arr);
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str_item);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", true_item);

    // step 3: Operate / Validate
    got_item = cJSON_GetArrayItem(arr, 0);
    is_str = cJSON_IsString(got_item);
    str_val = cJSON_GetStringValue(got_item);
    len = strlen(str_val) + 1;
    copy = (char *)cJSON_malloc(len);
    memset(copy, 0, len);
    memcpy(copy, str_val, len);
    copied_item = cJSON_CreateString(copy);
    cJSON_AddItemToObject(root, "copied", copied_item);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(copy);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}