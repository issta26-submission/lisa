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
//<ID> 784
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *num_pi = (cJSON *)0;
    cJSON *num_answer = (cJSON *)0;
    cJSON *direct = (cJSON *)0;
    cJSON *retrieved_data = (cJSON *)0;
    cJSON *retrieved_pi = (cJSON *)0;
    cJSON *retrieved_direct = (cJSON *)0;
    char *printed = (char *)0;
    double pi_val = 0.0;
    double direct_val = 0.0;
    cJSON_bool is_num_pi = 0;
    cJSON_bool is_num_direct = 0;
    cJSON_bool added_flag = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();

    // step 3: Configure
    num_pi = cJSON_CreateNumber(3.1415);
    num_answer = cJSON_CreateNumber(42.0);
    added_flag = cJSON_AddItemToObject(child, "pi", num_pi);
    (void)cJSON_AddItemToObject(child, "answer", num_answer);
    added_flag = cJSON_AddItemToObject(root, "data", child);
    direct = cJSON_AddNumberToObject(root, "direct", 7.0);

    // step 4: Operate
    retrieved_data = cJSON_GetObjectItem(root, "data");
    retrieved_pi = cJSON_GetObjectItemCaseSensitive(retrieved_data, "pi");
    retrieved_direct = cJSON_GetObjectItemCaseSensitive(root, "direct");
    pi_val = cJSON_GetNumberValue(retrieved_pi);
    direct_val = cJSON_GetNumberValue(retrieved_direct);
    is_num_pi = cJSON_IsNumber(retrieved_pi);
    is_num_direct = cJSON_IsNumber(retrieved_direct);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (retrieved_data != (cJSON *)0) + (retrieved_pi != (cJSON *)0) + (retrieved_direct != (cJSON *)0) + (printed != (char *)0) + (int)is_num_pi + (int)is_num_direct + (pi_val > 0.0) + (direct_val == 7.0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}