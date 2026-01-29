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
//<ID> 789
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *group = (cJSON *)0;
    cJSON *num_pi = (cJSON *)0;
    cJSON *num_answer = (cJSON *)0;
    cJSON *retrieved_group = (cJSON *)0;
    cJSON *retrieved_pi = (cJSON *)0;
    cJSON *retrieved_answer = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_pi = 0;
    cJSON_bool added_answer = 0;
    int validation_score = 0;
    double val_pi = 0.0;
    double val_answer = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    group = cJSON_CreateObject();

    // step 3: Configure
    cJSON_AddItemToObject(root, "group", group);
    num_pi = cJSON_CreateNumber(3.14159);
    num_answer = cJSON_CreateNumber(42.0);
    added_pi = cJSON_AddItemToObject(group, "pi", num_pi);
    added_answer = cJSON_AddItemToObject(group, "answer", num_answer);

    // step 4: Operate
    retrieved_group = cJSON_GetObjectItem(root, "group");
    retrieved_pi = cJSON_GetObjectItemCaseSensitive(retrieved_group, "pi");
    retrieved_answer = cJSON_GetObjectItemCaseSensitive(retrieved_group, "answer");
    val_pi = cJSON_GetNumberValue(retrieved_pi);
    val_answer = cJSON_GetNumberValue(retrieved_answer);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (retrieved_group != (cJSON *)0) + (retrieved_pi != (cJSON *)0) + (retrieved_answer != (cJSON *)0) + (printed != (char *)0) + (int)added_pi + (int)added_answer + (val_pi > 3.0) + (val_answer == 42.0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}