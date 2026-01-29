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
//<ID> 2466
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *active_item = cJSON_AddTrueToObject(meta, "active");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    char *printed = cJSON_Print(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed ? printed[0] : '\0';

    // step 3: Operate
    cJSON_bool is_false = cJSON_IsFalse(flag);
    buffer[1] = (char)(int)is_false;
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON *retrieved_count = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(retrieved_count);
    buffer[2] = (char)(int)count_val;
    char *printed2 = cJSON_Print(root);
    buffer[3] = printed2 ? printed2[0] : '\0';

    // step 4: Validate and Cleanup
    cJSON_bool meta_has_active = cJSON_HasObjectItem(meta, "active");
    buffer[4] = (char)(int)meta_has_active;
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}