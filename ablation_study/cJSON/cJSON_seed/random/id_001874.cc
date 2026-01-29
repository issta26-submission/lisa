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
//<ID> 1874
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *str_item = NULL;
    cJSON *got0 = NULL;
    char *out = NULL;
    const char *got_str = NULL;
    cJSON_bool is_arr = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    s1 = cJSON_CreateString("one");
    s2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", str_item);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Core operations
    got0 = cJSON_GetArrayItem(arr, 0);
    is_arr = cJSON_IsArray(arr);
    got_str = cJSON_GetStringValue(got0);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(s1 != NULL);
    validation ^= (int)(s2 != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(got0 != NULL);
    validation ^= (int)(is_arr != 0);
    validation ^= (int)(got_str != NULL);
    validation ^= (int)(out != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}