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
//<ID> 781
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
    cJSON *retrieved_data = (cJSON *)0;
    cJSON *retrieved_pi = (cJSON *)0;
    cJSON *retrieved_answer = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_pi = 0;
    cJSON_bool added_answer = 0;
    cJSON_bool added_child = 0;
    int validation_score = 0;
    double val_pi = 0.0;
    double val_answer = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();

    // step 3: Configure
    num_pi = cJSON_CreateNumber(3.14159);
    num_answer = cJSON_CreateNumber(42.0);
    added_pi = cJSON_AddItemToObject(child, "pi", num_pi);
    added_answer = cJSON_AddItemToObject(child, "answer", num_answer);
    added_child = cJSON_AddItemToObject(root, "data", child);

    // step 4: Operate
    retrieved_data = cJSON_GetObjectItem(root, "data");
    retrieved_pi = cJSON_GetObjectItemCaseSensitive(retrieved_data, "pi");
    retrieved_answer = cJSON_GetObjectItemCaseSensitive(retrieved_data, "answer");
    val_pi = cJSON_GetNumberValue(retrieved_pi);
    val_answer = cJSON_GetNumberValue(retrieved_answer);
    printed = cJSON_Print(root);

    // step 5: Validate
    validation_score = (root != (cJSON *)0) + (child != (cJSON *)0) + (num_pi != (cJSON *)0) + (num_answer != (cJSON *)0) + (int)added_pi + (int)added_answer + (int)added_child + (retrieved_data != (cJSON *)0) + (retrieved_pi != (cJSON *)0) + (retrieved_answer != (cJSON *)0) + (int)cJSON_IsNumber(retrieved_pi) + (int)cJSON_IsNumber(retrieved_answer) + (printed != (char *)0);
    (void)validation_score;
    (void)val_pi;
    (void)val_answer;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}