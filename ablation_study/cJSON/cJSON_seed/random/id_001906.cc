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
//<ID> 1906
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
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *flag_item = NULL;
    cJSON *got_num1 = NULL;
    cJSON *got_num2 = NULL;
    cJSON *got_flag = NULL;
    cJSON_bool add_item_result = 0;
    cJSON_bool is_num1 = 0;
    cJSON_bool is_num2 = 0;
    cJSON_bool is_flag = 0;
    double v1 = 0.0;
    double v2 = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num1 = cJSON_AddNumberToObject(root, "pi", 3.1415);
    num2 = cJSON_CreateNumber(42.0);
    add_item_result = cJSON_AddItemToObject(root, "answer", num2);
    flag_item = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Core operations
    got_num1 = cJSON_GetObjectItem(root, "pi");
    got_num2 = cJSON_GetObjectItem(root, "answer");
    got_flag = cJSON_GetObjectItem(root, "enabled");
    v1 = cJSON_GetNumberValue(got_num1);
    v2 = cJSON_GetNumberValue(got_num2);
    is_num1 = cJSON_IsNumber(got_num1);
    is_num2 = cJSON_IsNumber(got_num2);
    is_flag = cJSON_IsTrue(got_flag);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(add_item_result != 0);
    validation ^= (int)(flag_item != NULL);
    validation ^= (int)(got_num1 != NULL);
    validation ^= (int)(got_num2 != NULL);
    validation ^= (int)(got_flag != NULL);
    validation ^= (int)(is_num1 != 0);
    validation ^= (int)(is_num2 != 0);
    validation ^= (int)(is_flag != 0);
    validation ^= (int)(v2 == 42.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}