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
//<ID> 785
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *metrics = (cJSON *)0;
    cJSON *num_pi = (cJSON *)0;
    cJSON *num_answer = (cJSON *)0;
    cJSON *retrieved_metrics = (cJSON *)0;
    cJSON *retrieved_pi = (cJSON *)0;
    char *printed = (char *)0;
    double pi_value = 0.0;
    int validation_score = 0;
    cJSON_bool is_number = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    metrics = cJSON_CreateObject();
    num_pi = cJSON_CreateNumber(3.14159);
    num_answer = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToObject(metrics, "pi", num_pi);
    cJSON_AddItemToObject(metrics, "answer", num_answer);
    cJSON_AddItemToObject(root, "metrics", metrics);

    // step 4: Operate
    retrieved_metrics = cJSON_GetObjectItem(root, "metrics");
    retrieved_pi = cJSON_GetObjectItemCaseSensitive(retrieved_metrics, "pi");
    pi_value = cJSON_GetNumberValue(retrieved_pi);
    is_number = cJSON_IsNumber(retrieved_pi);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (printed != (char *)0) + (retrieved_metrics != (cJSON *)0) + (retrieved_pi != (cJSON *)0) + (pi_value > 3.0) + (int)is_number;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}