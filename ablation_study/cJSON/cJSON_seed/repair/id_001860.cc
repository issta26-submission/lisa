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
//<ID> 1860
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
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(3.0));
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddItemToObject(meta, "label", cJSON_CreateString("initial"));

    // step 2: Configure
    cJSON *label_item = cJSON_GetObjectItem(meta, "label");
    char *set_ret = cJSON_SetValuestring(label_item, "updated");
    (void)set_ret;
    cJSON_AddItemToObject(root, "extra", cJSON_CreateString("value"));

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    int size_before = cJSON_GetArraySize(items);
    cJSON_bool flag_true = cJSON_IsTrue(flag);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    double first_val = cJSON_GetNumberValue(first);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (flag_true % 10));
    buffer[2] = (char)('0' + (size_before % 10));
    buffer[3] = (char)('0' + (((int)first_val) % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}