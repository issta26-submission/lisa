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
//<ID> 1565
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
    cJSON *retrieved = NULL;
    cJSON *true_item = NULL;
    cJSON_bool is_str = 0;
    cJSON *is_str_item = NULL;
    const char *strval = NULL;
    char *copystr = NULL;
    char *out = NULL;
    size_t len = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "myArray", arr);
    str0 = cJSON_CreateString("alpha");
    str1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddItemToArray(arr, str1);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Operate / Validate
    retrieved = cJSON_GetArrayItem(arr, 0);
    is_str = cJSON_IsString(retrieved);
    is_str_item = cJSON_CreateNumber((double)is_str);
    cJSON_AddItemToObject(root, "is_string", is_str_item);
    strval = cJSON_GetStringValue(retrieved);
    len = strlen(strval) + 1;
    copystr = (char *)cJSON_malloc(len);
    memset(copystr, 0, len);
    memcpy(copystr, strval, len);
    cJSON_AddItemToObject(root, "copied", cJSON_CreateString(copystr));
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(copystr);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}