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
//<ID> 894
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *t = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, t);
    cJSON *n = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, n);
    cJSON *s = cJSON_CreateString("example");
    cJSON_AddItemToArray(items, s);
    cJSON *id = cJSON_CreateString("node-1");
    cJSON_AddItemToObject(meta, "id", id);

    // step 3: Operate and Validate
    size_t buf_len = 512;
    char *buffer = (char *)cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)buf_len, 1);
    int count = cJSON_GetArraySize(items);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_true = cJSON_IsTrue(first);
    double second_num = cJSON_GetNumberValue(cJSON_GetArrayItem(items, 1));
    const char *str_val = cJSON_GetStringValue(s);
    double str_len = (double)(str_val ? (int)strlen(str_val) : 0);
    cJSON_AddNumberToObject(meta, "count", (double)count);
    cJSON_AddNumberToObject(meta, "first_true", (double)first_true);
    cJSON_AddNumberToObject(meta, "second_value", second_num);
    cJSON_AddNumberToObject(meta, "str_length", str_len);
    cJSON_AddNumberToObject(meta, "printed_flag", (double)printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}