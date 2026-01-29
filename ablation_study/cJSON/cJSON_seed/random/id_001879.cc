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
//<ID> 1879
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
    cJSON *str = NULL;
    cJSON *arr_item = NULL;
    cJSON *bool_item = NULL;
    cJSON *got_item = NULL;
    cJSON_bool arr_check = 0;
    char *out = NULL;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    str = cJSON_CreateString("hello");
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "greeting", str);
    cJSON_AddItemToObject(root, "list", arr);
    bool_item = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Core operations
    arr_item = cJSON_CreateString("member");
    cJSON_AddItemToArray(arr, arr_item);
    got_item = cJSON_GetArrayItem(arr, 0);
    arr_check = cJSON_IsArray(arr);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(str != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(arr_item != NULL);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(got_item != NULL);
    validation ^= (int)(got_item == arr_item);
    validation ^= (int)(arr_check != 0);
    validation ^= (int)(out != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}