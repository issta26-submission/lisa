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
//<ID> 1824
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *entry = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "entry", entry);
    cJSON *name_item = cJSON_CreateString("delta");
    cJSON_AddItemToObject(entry, "name", name_item);

    // step 2: Configure
    const char *ver = cJSON_Version();
    cJSON_AddItemToObject(root, "version", cJSON_CreateString(ver));
    cJSON_AddNumberToObject(meta, "count", 7.0);
    cJSON_AddNumberToObject(entry, "score", 99.5);

    // step 3: Operate and Validate
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON *count_item = cJSON_GetObjectItem(meta_item, "count");
    double count = cJSON_GetNumberValue(count_item);
    cJSON *entry_obj = cJSON_GetObjectItem(root, "entry");
    cJSON *name_obj = cJSON_GetObjectItem(entry_obj, "name");
    const char *name_str = cJSON_GetStringValue(name_obj);
    cJSON *ver_obj = cJSON_GetObjectItem(root, "version");
    const char *ver_str = cJSON_GetStringValue(ver_obj);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = name_str ? name_str[0] : 'X';
    buffer[1] = (char)('0' + (((int)count) % 10));
    buffer[2] = ver_str ? ver_str[0] : 'V';
    (void)buffer; (void)name_str; (void)ver_str; (void)count; (void)name_obj; (void)entry_obj;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}