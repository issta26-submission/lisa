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
//<ID> 1857
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *num_item = NULL;
    cJSON *false_item = NULL;
    cJSON *retrieved_num = NULL;
    cJSON *retrieved_flag = NULL;
    char *out = NULL;
    double num_value = 0.0;
    cJSON_bool flag_is_false = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(child, "answer", num_item);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(child, "flag", false_item);

    // step 3: Core operations
    out = cJSON_PrintUnformatted(root);
    retrieved_num = cJSON_GetObjectItem(child, "answer");
    num_value = cJSON_GetNumberValue(retrieved_num);
    retrieved_flag = cJSON_GetObjectItem(child, "flag");
    flag_is_false = cJSON_IsFalse(retrieved_flag);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(child != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(retrieved_num != NULL);
    validation ^= (int)(num_value == 42.0);
    validation ^= (int)(flag_is_false);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}