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
//<ID> 1822
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *ver = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *first = cJSON_CreateObject();
    cJSON_AddItemToObject(first, "name", cJSON_CreateString("alpha"));
    cJSON_AddItemToObject(first, "active", cJSON_CreateTrue());
    cJSON_AddItemToArray(items, first);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "root_version", 2.5);
    cJSON_AddNumberToObject(first, "index", 0.0);
    cJSON_AddItemToObject(meta, "description", cJSON_CreateString("sample"));
    cJSON *retrieved_meta = cJSON_GetObjectItem(root, "meta");

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    const char *name = cJSON_GetStringValue(cJSON_GetObjectItem(first, "name"));
    double root_ver = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "root_version"));
    const char *meta_desc = cJSON_GetStringValue(cJSON_GetObjectItem(retrieved_meta, "description"));
    buffer[0] = name ? name[0] : 'X';
    buffer[1] = (char)('0' + (((int)root_ver) % 10));
    buffer[2] = meta_desc ? meta_desc[0] : 'Y';
    buffer[3] = ver ? ver[0] : 'V';
    (void)buffer; (void)name; (void)root_ver; (void)meta_desc; (void)ver;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}