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
//<ID> 1876
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[48];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *str_item = NULL;
    cJSON *got_item = NULL;
    cJSON *bool_item = NULL;
    char *out = NULL;
    const char *got_str = NULL;
    cJSON_bool is_array = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str_item);
    bool_item = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Core operations
    got_item = cJSON_GetArrayItem(arr, 0);
    is_array = cJSON_IsArray(arr);
    out = cJSON_PrintUnformatted(root);
    got_str = cJSON_GetStringValue(got_item);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(got_item != NULL);
    validation ^= (int)(is_array != 0);
    validation ^= (int)(out != NULL);
    validation ^= (int)(got_str != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}