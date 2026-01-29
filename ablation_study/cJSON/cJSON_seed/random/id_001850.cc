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
//<ID> 1850
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
    cJSON *num_item = NULL;
    cJSON *false_item = NULL;
    cJSON *nested = NULL;
    cJSON *retrieved_answer = NULL;
    cJSON *retrieved_flag = NULL;
    cJSON *double_item = NULL;
    char *out = NULL;
    double answer_value = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "answer", num_item);
    cJSON_AddItemToObject(root, "is_valid", false_item);
    nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "nested", nested);

    // step 3: Core operations
    retrieved_answer = cJSON_GetObjectItem(root, "answer");
    retrieved_flag = cJSON_GetObjectItem(root, "is_valid");
    answer_value = cJSON_GetNumberValue(retrieved_answer);
    double_item = cJSON_CreateNumber(answer_value * 2.0);
    cJSON_AddItemToObject(nested, "double", double_item);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(nested != NULL);
    validation ^= (int)(retrieved_answer != NULL);
    validation ^= (int)(retrieved_flag != NULL);
    validation ^= (int)(answer_value == 42.0);
    validation ^= (int)(cJSON_IsFalse(retrieved_flag));
    validation ^= (int)(out != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}