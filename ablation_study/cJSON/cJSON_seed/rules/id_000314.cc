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
//<ID> 314
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "items");
    cJSON *added_true = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *added_false = cJSON_AddBoolToObject(root, "disabled", 0);
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(7.5);
    cJSON *child = cJSON_CreateObject();
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool is_false = 0;
    cJSON_bool is_invalid = 0;
    cJSON *missing = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "inner", cJSON_CreateString("value"));

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    is_false = cJSON_IsFalse(added_false);
    missing = cJSON_GetObjectItem(root, "nonexistent_key");
    is_invalid = cJSON_IsInvalid(missing);
    cJSON_PrintPreallocated(root, buffer, 256, 0);
    buffer[0] = (char)('0' + (int)is_false);
    buffer[1] = (char)('0' + (int)is_invalid);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}