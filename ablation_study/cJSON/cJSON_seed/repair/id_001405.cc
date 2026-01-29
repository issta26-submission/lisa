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
//<ID> 1405
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "count", 2.0);
    cJSON_AddTrueToObject(root, "active");
    cJSON *arr = cJSON_AddArrayToObject(root, "numbers");
    cJSON *num0 = cJSON_CreateNumber(10.0);
    cJSON *num1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(num1, 1);
    cJSON_SetNumberHelper(dup, 30.5);
    cJSON_AddNumberToObject(root, "replaced_with", 30.5);

    // step 3: Operate and Validate
    cJSON_ReplaceItemInArray(arr, 0, dup);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 0);
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    char *s = (char *)scratch;
    s[0] = buf[0];
    s[1] = (char)('0' + ((int)first_val % 10));
    s[2] = (char)('0' + ((int)cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count")) % 10));

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}