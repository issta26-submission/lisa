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
//<ID> 1401
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
    cJSON *item0 = cJSON_CreateObject();
    cJSON_AddNumberToObject(item0, "value", 42.5);
    cJSON_AddTrueToObject(item0, "flag");
    cJSON_AddItemToArray(items, item0);
    cJSON *item1 = cJSON_CreateObject();
    cJSON_AddNumberToObject(item1, "value", 7.0);
    cJSON_AddTrueToObject(item1, "flag");
    cJSON_AddItemToArray(items, item1);

    // step 2: Configure
    cJSON *dup_item = cJSON_Duplicate(item0, 1);

    // step 3: Operate and Validate
    cJSON_bool replaced = cJSON_ReplaceItemInArray(items, 1, dup_item);
    cJSON *replaced_node = cJSON_GetArrayItem(items, 1);
    cJSON *value_node = cJSON_GetObjectItem(replaced_node, "value");
    double value = cJSON_GetNumberValue(value_node);
    cJSON *flag_node = cJSON_GetObjectItem(replaced_node, "flag");
    cJSON_bool flag_true = cJSON_IsTrue(flag_node);
    int buf_len = 128;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buf, buf_len, 0);
    void *scratch = cJSON_malloc(16);
    memset(scratch, 0, 16);
    char *s = (char *)scratch;
    s[0] = (char)('0' + ((int)value % 10));
    s[1] = flag_true ? '1' : '0';
    s[2] = printed ? 'P' : 'N';

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}