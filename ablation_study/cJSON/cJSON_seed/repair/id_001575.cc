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
//<ID> 1575
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(123.5);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(child, "num", num);
    cJSON_AddItemToObject(child, "flag", flag);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *retrieved_child = cJSON_GetObjectItem(root, "child");
    cJSON_bool is_obj = cJSON_IsObject(retrieved_child);
    cJSON *retrieved_num = cJSON_GetObjectItem(retrieved_child, "num");
    double val = cJSON_GetNumberValue(retrieved_num);
    cJSON *mirror_num = cJSON_CreateNumber(val * 2.0);
    cJSON_AddItemToObject(root, "mirror", mirror_num);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = (char)('0' + (int)is_obj);
    scratch[1] = out_len ? out[0] : ' ';
    scratch[2] = (char)('0' + (int)((int)val % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}