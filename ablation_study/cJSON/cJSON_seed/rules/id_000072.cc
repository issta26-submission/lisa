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
//<ID> 72
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_obj = cJSON_CreateNumber(42.0);
    cJSON *msg_obj = cJSON_CreateString("world");
    cJSON *bonus_obj = NULL;
    cJSON *retrieved_num = NULL;
    cJSON *retrieved_bonus = NULL;
    cJSON *retrieved_msg = NULL;
    cJSON *sum_obj = NULL;
    double num_val = 0.0;
    double bonus_val = 0.0;
    double sum_val = 0.0;
    char *msg_val = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "num", num_obj);
    cJSON_AddItemToObject(root, "msg", msg_obj);
    bonus_obj = cJSON_AddNumberToObject(root, "bonus", 3.5);

    // step 3: Operate / Validate
    retrieved_num = cJSON_GetObjectItem(root, "num");
    retrieved_bonus = cJSON_GetObjectItem(root, "bonus");
    retrieved_msg = cJSON_GetObjectItem(root, "msg");
    num_val = cJSON_GetNumberValue(retrieved_num);
    bonus_val = cJSON_GetNumberValue(retrieved_bonus);
    msg_val = cJSON_GetStringValue(retrieved_msg);
    sum_val = num_val + bonus_val;
    sum_obj = cJSON_AddNumberToObject(root, "sum", sum_val);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}