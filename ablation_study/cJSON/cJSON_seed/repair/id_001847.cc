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
//<ID> 1847
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "version", cJSON_CreateString("1.2.3"));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(30.0));

    // step 2: Configure
    const char *libver = cJSON_Version();
    char *printed = cJSON_PrintUnformatted(root);
    int size_before = cJSON_GetArraySize(items);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *dup_meta = cJSON_GetObjectItem(dup, "meta");
    cJSON *dup_ver = cJSON_GetObjectItem(dup_meta, "version");
    cJSON_SetValuestring(dup_ver, "1.2.3-dup");

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(items, 1);
    double detached_val = cJSON_GetNumberValue(detached);
    cJSON_AddItemToObject(meta, "detached", cJSON_CreateNumber(detached_val));
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_version = cJSON_GetObjectItem(got_meta, "version");
    const char *version_str = cJSON_GetStringValue(got_version);
    int size_after = cJSON_GetArraySize(items);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = libver ? libver[0] : 'V';
    buffer[2] = version_str ? version_str[0] : 'v';
    buffer[3] = (char)('0' + (size_before % 10));
    buffer[4] = (char)('0' + (size_after % 10));
    buffer[5] = (char)('0' + (((int)detached_val) % 10));
    (void)buffer; (void)libver; (void)version_str; (void)size_before; (void)size_after; (void)detached_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}