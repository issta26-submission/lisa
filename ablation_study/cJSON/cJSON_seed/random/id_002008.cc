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
//<ID> 2008
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
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *bool_item = NULL;
    char *sval = NULL;
    char *printed = NULL;
    cJSON_bool added_num = 0;
    cJSON_bool added_str = 0;
    int arr_size = 0;
    int validation = 0;

    // step 2: Setup (initialize root, array and items)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(42.0);
    str_item = cJSON_CreateString("sample");

    // step 3: Core operations (add items to array and object, add bool using required API)
    added_num = cJSON_AddItemToArray(arr, num_item);
    added_str = cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "items", arr);
    bool_item = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 4: Inspect / Use required APIs (get array size and string value, print)
    arr_size = cJSON_GetArraySize(arr);
    sval = cJSON_GetStringValue(str_item);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate (aggregate simple checks into buffer)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(added_num != 0);
    validation ^= (int)(added_str != 0);
    validation ^= (int)(arr_size >= 2);
    validation ^= (int)(sval != NULL);
    validation ^= (int)(printed != NULL);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}