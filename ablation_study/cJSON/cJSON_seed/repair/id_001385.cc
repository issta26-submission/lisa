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
//<ID> 1385
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddNumberToObject(root, "a", 10.5);
    cJSON_AddNumberToObject(root, "b", 20.25);

    // step 3: Operate and Validate
    cJSON *a_item = cJSON_GetObjectItem(root, "a");
    double a_val = cJSON_GetNumberValue(a_item);
    cJSON *b_item = cJSON_GetObjectItem(root, "b");
    double b_val = cJSON_GetNumberValue(b_item);
    double sum = a_val + b_val;
    cJSON *sum_item = cJSON_AddNumberToObject(root, "sum", sum);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buf, buf_len, 0);
    buf[buf_len - 1] = (char)('0' + (int)printed);
    double read_sum = cJSON_GetNumberValue(sum_item);
    double adjusted = read_sum + (double)(int)printed;
    cJSON_AddNumberToObject(root, "adjusted", adjusted);

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}