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
//<ID> 1714
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "fuzzer");
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, flag);
    cJSON *entry1 = cJSON_CreateObject();
    cJSON_AddNumberToObject(entry1, "id", 1.0);
    cJSON_AddStringToObject(entry1, "label", "one");
    cJSON_AddItemToArray(items, entry1);

    // step 2: Configure
    cJSON *entry2 = cJSON_CreateObject();
    cJSON_AddNumberToObject(entry2, "id", 2.0);
    cJSON_AddStringToObject(entry2, "label", "two");
    cJSON_AddItemToArray(items, entry2);
    cJSON_AddStringToObject(root, "status", "configured");

    // step 3: Operate and Validate
    cJSON *retr_items = cJSON_GetObjectItem(root, "items");
    cJSON_bool items_is_array = cJSON_IsArray(retr_items);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(64);
    memset(tmp, 0, 64);
    char *buf = (char *)tmp;
    buf[0] = printed ? printed[0] : 'X';
    buf[1] = (char)('0' + (int)items_is_array);
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    const char *name_val = cJSON_GetStringValue(got_name);
    buf[2] = name_val ? name_val[0] : 'Y';
    (void)buf;
    (void)retr_items;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}