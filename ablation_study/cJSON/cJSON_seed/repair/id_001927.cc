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
//<ID> 1927
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
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, true_item);
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, num_item);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(items, str_item);
    cJSON *null_item = cJSON_AddNullToObject(root, "optional");

    // step 2: Configure
    cJSON_SetValuestring(str_item, "updated");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "enabled", cJSON_CreateTrue());

    // step 3: Operate and Validate
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    int arr_size = cJSON_GetArraySize(items);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    const char *sval = cJSON_GetStringValue(str_item);
    char marker0 = printed_ok ? buffer[0] : 'X';
    char marker1 = (sval != NULL) ? sval[0] : 'X';
    buffer[0] = marker0;
    buffer[1] = (char)('0' + (arr_size % 10));
    buffer[2] = marker1;
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}