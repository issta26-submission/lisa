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
//<ID> 1718
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
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", active);

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON *num = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(items, num);

    // step 3: Operate and Validate
    cJSON_bool is_arr = cJSON_IsArray(items);
    cJSON *retr_active = cJSON_GetObjectItem(root, "active");
    int arr_size = cJSON_GetArraySize(items);
    char *printed = cJSON_PrintUnformatted(root);
    int buf_len = 512;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    char summary[8];
    memset(summary, 0, sizeof(summary));
    summary[0] = buf[0];
    summary[1] = (char)('0' + (int)(is_arr));
    summary[2] = (char)('0' + (arr_size % 10));
    summary[3] = (char)('0' + (retr_active->type % 10));
    (void)summary;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}