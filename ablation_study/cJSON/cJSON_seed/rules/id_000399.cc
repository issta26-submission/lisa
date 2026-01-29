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
//<ID> 399
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *str_item = cJSON_CreateString("alpha");
    cJSON *active_item = NULL;
    cJSON *printed = NULL;
    cJSON *parsed = NULL;
    cJSON *name_item = NULL;
    cJSON *raw_retrieved = NULL;
    const char *name_str = NULL;
    cJSON_bool is_active = 0;
    cJSON_bool is_raw = 0;

    // step 2: Setup / Configure
    active_item = cJSON_AddBoolToObject(root, "active", 1);
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON_AddItemToObject(root, "name", str_item);

    // step 3: Operate / Validate
    printed = (cJSON *)cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse((const char *)printed);
    name_item = cJSON_GetObjectItem(parsed, "name");
    raw_retrieved = cJSON_GetObjectItem(parsed, "raw");
    active_item = cJSON_GetObjectItem(parsed, "active");
    name_str = cJSON_GetStringValue(name_item);
    is_active = cJSON_IsTrue(active_item);
    is_raw = cJSON_IsRaw(raw_retrieved);
    buffer[0] = name_str[0];
    buffer[1] = (char)('0' + (int)is_active);
    buffer[2] = (char)('0' + (int)is_raw);
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}