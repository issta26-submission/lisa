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
//<ID> 1431
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"meta\":{\"id\":123},\"data\":[{\"name\":\"Alice\"},{\"name\":\"Bob\"}],\"raw\":\"<raw>text</raw>\"}";
    size_t json_len = strlen(json_text);
    cJSON *root = cJSON_ParseWithLength(json_text, json_len);

    // step 2: Configure
    cJSON *data = cJSON_GetObjectItem(root, "data");
    cJSON *first_item = cJSON_GetArrayItem(data, 0);
    cJSON *dup_item = cJSON_Duplicate(first_item, 1);
    cJSON *detached_item = cJSON_DetachItemViaPointer(data, first_item);
    cJSON_AddItemToArray(data, dup_item);
    cJSON_AddRawToObject(root, "injected_raw", "{\"injected\":true}");

    // step 3: Operate & Validate
    const int buf_len = 1024;
    char *buffer = (char *)cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    cJSON_bool ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    int new_size = cJSON_GetArraySize(data);
    cJSON *last_item = cJSON_GetArrayItem(data, new_size - 1);
    cJSON *name_obj = cJSON_GetObjectItem(last_item, "name");
    const char *name_val = cJSON_GetStringValue(name_obj);
    (void)ok;
    (void)name_val;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(detached_item);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}