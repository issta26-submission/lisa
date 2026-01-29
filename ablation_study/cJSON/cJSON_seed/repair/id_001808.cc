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
//<ID> 1808
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON *first = cJSON_CreateObject();
    cJSON_AddItemToObject(first, "name", cJSON_CreateString("one"));
    cJSON_AddItemToObject(first, "type", cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(list, first);

    // step 2: Configure
    cJSON *second = cJSON_CreateObject();
    cJSON_AddItemToObject(second, "name", cJSON_CreateString("two"));
    cJSON_AddItemToObject(second, "type", cJSON_CreateString("beta"));
    cJSON_AddItemToArray(list, second);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "count", 2.0);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    cJSON *count_item = cJSON_GetObjectItem(meta_ref, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON *list_ref = cJSON_GetObjectItem(root, "list");
    cJSON *first_item = cJSON_GetArrayItem(list_ref, 0);
    cJSON *name_item = cJSON_GetObjectItem(first_item, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_str ? name_str[0] : 'Z';
    buffer[2] = (char)('0' + ((int)count_val % 10));
    (void)buffer; (void)name_str; (void)count_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}