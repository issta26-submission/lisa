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
//<ID> 311
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON *arr = NULL;
    cJSON *enabled_item = NULL;
    cJSON *disabled_item = NULL;
    cJSON *invalid_item = cJSON_CreateNull();
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    double first_num = 0.0;
    cJSON_bool disabled_is_false = 0;
    cJSON_bool invalid_check = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "config", cfg);
    enabled_item = cJSON_AddBoolToObject(cfg, "enabled", 1);
    disabled_item = cJSON_AddBoolToObject(cfg, "disabled", 0);
    arr = cJSON_AddArrayToObject(root, "items");
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("example"));

    // step 3: Operate / Validate
    disabled_is_false = cJSON_IsFalse(disabled_item);
    invalid_item->type = 0x7FFFFFFF;
    invalid_check = cJSON_IsInvalid(invalid_item);
    printed = cJSON_PrintUnformatted(root);
    first_num = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    buffer[0] = (char)('0' + (int)disabled_is_false);
    buffer[1] = (char)('A' + ((int)first_num % 26));
    (void)invalid_check;
    (void)first_num;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(invalid_item);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}