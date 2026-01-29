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
//<ID> 22
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *num1 = cJSON_CreateNumber(1.234);
    cJSON *num2 = cJSON_CreateNumber(5.678);
    cJSON *flag = cJSON_CreateTrue();
    cJSON *retrieved_flag = NULL;
    cJSON *retrieved_num = NULL;
    int flag_true = 0;
    int num_is_number = 0;
    int validation = 0;

    // step 2: Setup
    cJSON_AddItemToObject(root, "value", num1);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate/Validate
    cJSON_ReplaceItemViaPointer(root, num1, num2);
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    flag_true = (int)cJSON_IsTrue(retrieved_flag);
    retrieved_num = cJSON_GetObjectItem(root, "value");
    num_is_number = (int)cJSON_IsNumber(retrieved_num);
    validation = flag_true * 10 + num_is_number;
    cJSON_AddItemToObject(root, "validation", cJSON_CreateNumber((double)validation));

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}