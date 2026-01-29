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
//<ID> 312
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *items = NULL;
    cJSON *bool_ptr = NULL;
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON_bool is_false = 0;
    cJSON_bool is_invalid = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "settings", settings);
    items = cJSON_AddArrayToObject(settings, "items");
    cJSON_AddItemToObject(root, "extra", arr);
    bool_ptr = cJSON_AddBoolToObject(settings, "enabled", 0);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    is_false = cJSON_IsFalse(bool_ptr);
    is_invalid = cJSON_IsInvalid(root);
    buffer[0] = (char)('0' + (int)is_false);
    buffer[1] = (char)('A' + (int)is_invalid);
    buffer[2] = printed ? printed[0] : 'X';
    (void)items;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}