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
//<ID> 1560
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
    cJSON *first = NULL;
    cJSON *dup_str = NULL;
    char *json_text = NULL;
    const char *first_val = NULL;
    char *copied = NULL;
    size_t len = 0;
    cJSON_bool is_str = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    item0 = cJSON_CreateString("alpha");
    item1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToObject(root, "flag", cJSON_CreateTrue());

    // step 3: Operate / Validate
    first = cJSON_GetArrayItem(arr, 0);
    is_str = cJSON_IsString(first);
    first_val = cJSON_GetStringValue(first);
    len = strlen(first_val) + 1;
    copied = (char *)cJSON_malloc(len);
    memset(copied, 0, len);
    memcpy(copied, first_val, len);
    dup_str = cJSON_CreateString(copied);
    cJSON_AddItemToObject(root, "copied", dup_str);
    cJSON_AddItemToObject(root, "first_is_string", cJSON_CreateBool(is_str));
    json_text = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(json_text);
    cJSON_free(copied);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}