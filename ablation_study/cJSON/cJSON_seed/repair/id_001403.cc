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
//<ID> 1403
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
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(items, n0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddNumberToObject(root, "initial_sum", 30.0);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "active");
    cJSON *dup = cJSON_Duplicate(items, 1);

    // step 3: Operate and Validate
    cJSON *replacement = cJSON_Duplicate(cJSON_GetArrayItem(dup, 1), 1);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(items, 0, replacement);
    (void)replaced;
    cJSON *item0 = cJSON_GetArrayItem(items, 0);
    cJSON *item1 = cJSON_GetArrayItem(items, 1);
    double val0 = cJSON_GetNumberValue(item0);
    double val1 = cJSON_GetNumberValue(item1);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 0);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    char *s = (char *)scratch;
    s[0] = buf[0];
    s[1] = (char)('0' + ((int)val0 % 10));
    s[2] = (char)('0' + ((int)val1 % 10));

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(scratch);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}