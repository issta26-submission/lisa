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
//<ID> 1408
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
    cJSON *item_a = cJSON_CreateNumber(42.0);
    cJSON *item_b = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(items, item_a);
    cJSON_AddItemToArray(items, item_b);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(items));
    cJSON_AddTrueToObject(root, "valid");
    cJSON *dup_a = cJSON_Duplicate(item_a, 1);

    // step 3: Operate and Validate
    cJSON_ReplaceItemInArray(items, 1, dup_a);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    double v_first = cJSON_GetNumberValue(first);
    double v_second = cJSON_GetNumberValue(second);
    char *json_text = cJSON_PrintUnformatted(root);
    int buf_len = 128;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    buf[0] = (char)('0' + ((int)v_first % 10));
    buf[1] = (char)('0' + ((int)v_second % 10));
    buf[2] = json_text[0];
    buf[3] = json_text[1];

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(json_text);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}