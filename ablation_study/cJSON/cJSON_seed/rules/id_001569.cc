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
//<ID> 1569
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
    cJSON *flag = NULL;
    cJSON *first = NULL;
    cJSON *copied = NULL;
    const char *first_val = NULL;
    cJSON_bool first_is_string = 0;
    char *out = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    str0 = cJSON_CreateString("alpha");
    str1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddItemToArray(arr, str1);
    flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 3: Operate / Validate
    first = cJSON_GetArrayItem(arr, 0);
    first_is_string = cJSON_IsString(first);
    first_val = cJSON_GetStringValue(first);
    copied = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "first_copy", copied);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}