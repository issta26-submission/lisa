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
//<ID> 1875
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "active", active);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *details = cJSON_AddObjectToObject(meta, "details");
    cJSON *count = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(details, "count", count);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(meta, "name");
    char *set_ret = cJSON_SetValuestring(name_item, "example2");
    cJSON_bool is_str = cJSON_IsString(name_item);
    cJSON_bool is_obj = cJSON_IsObject(details);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = set_ret ? set_ret[0] : 'Y';
    buffer[2] = (char)('0' + (is_str % 10));
    double count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(details, "count"));
    buffer[3] = (char)('0' + (((int)count_val) % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}