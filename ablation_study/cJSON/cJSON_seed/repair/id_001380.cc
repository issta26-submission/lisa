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
//<ID> 1380
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "a", 3.14);
    cJSON_AddNumberToObject(root, "b", 2.0);

    // step 2: Configure
    cJSON *a_item = cJSON_GetObjectItem(root, "a");
    cJSON *b_item = cJSON_GetObjectItem(root, "b");
    double a_val = cJSON_GetNumberValue(a_item);
    double b_val = cJSON_GetNumberValue(b_item);
    double sum = a_val + b_val;
    cJSON_AddNumberToObject(root, "sum", sum);

    // step 3: Operate and Validate
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buf, buf_len, 0);
    cJSON *sum_item = cJSON_GetObjectItem(root, "sum");
    double sum_read = cJSON_GetNumberValue(sum_item);
    double augmented = sum_read + (double)printed + (double)(unsigned char)buf[0];
    cJSON_AddNumberToObject(root, "augmented", augmented);

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}