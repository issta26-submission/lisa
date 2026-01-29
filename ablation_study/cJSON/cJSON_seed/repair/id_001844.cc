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
//<ID> 1844
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON_AddItemToArray(data, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(data, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(data, cJSON_CreateNumber(30.0));
    cJSON_AddItemToObject(root, "data", data);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "version", cJSON_CreateString("1.2.3"));
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Operate
    cJSON *got_data = cJSON_GetObjectItem(root, "data");
    cJSON *detached = cJSON_DetachItemFromArray(got_data, 1);
    cJSON *moved_holder = cJSON_CreateObject();
    cJSON_AddItemToObject(moved_holder, "moved", detached);

    // step 3: Validate
    char *printed = cJSON_PrintUnformatted(root);
    int size_after = cJSON_GetArraySize(got_data);
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *ver_item = cJSON_GetObjectItem(got_meta, "version");
    const char *ver_str = cJSON_GetStringValue(ver_item);
    char *buffer = (char *)cJSON_malloc((int)strlen(printed) + 1);
    memset(buffer, 0, (int)strlen(printed) + 1);
    strcpy(buffer, printed);
    buffer[0] = ver_str ? ver_str[0] : 'X';
    (void)size_after;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(moved_holder);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}