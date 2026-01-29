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
//<ID> 1207
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateStringReference("alpha");
    cJSON *s2 = cJSON_CreateStringReference("beta");

    // step 2: Configure
    cJSON_InsertItemInArray(arr, 0, s1);
    cJSON_InsertItemInArray(arr, 1, s2);

    // step 3: Operate
    char *printed = cJSON_Print(arr);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool pre_ok = cJSON_PrintPreallocated(arr, buf, buf_len, 1);
    int success_sum = (printed != NULL) + (int)pre_ok;
    arr->valueint = success_sum;

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(arr);
    // API sequence test completed successfully
    return 66;
}