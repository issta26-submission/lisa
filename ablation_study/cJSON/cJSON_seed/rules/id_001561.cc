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
//<ID> 1561
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
    cJSON *str0 = NULL;
    cJSON *str1 = NULL;
    cJSON *true_item = NULL;
    cJSON *retrieved0 = NULL;
    cJSON *copied_item = NULL;
    const char *val0 = NULL;
    char *copied = NULL;
    char *out = NULL;
    size_t len = 0;
    cJSON_bool is_str = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", arr);
    str0 = cJSON_CreateString("alpha");
    str1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddItemToArray(arr, str1);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_item);

    // step 3: Operate / Validate
    retrieved0 = cJSON_GetArrayItem(arr, 0);
    is_str = cJSON_IsString(retrieved0);
    val0 = cJSON_GetStringValue(retrieved0);
    len = strlen(val0) + 1;
    copied = (char *)cJSON_malloc(len);
    memset(copied, 0, len);
    memcpy(copied, val0, len);
    copied_item = cJSON_CreateString(copied);
    cJSON_AddItemToObject(root, "copied_value", copied_item);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(copied);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}