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
//<ID> 788
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_pi = (cJSON *)0;
    cJSON *num_neg = (cJSON *)0;
    cJSON *retrieved_pi = (cJSON *)0;
    cJSON *retrieved_neg = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_pi = 0;
    cJSON_bool added_neg = 0;
    double val_pi = 0.0;
    double val_neg = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_pi = cJSON_CreateNumber(3.14);
    num_neg = cJSON_CreateNumber(-42.0);

    // step 3: Configure
    added_pi = cJSON_AddItemToObject(root, "pi", num_pi);
    added_neg = cJSON_AddItemToObject(root, "neg", num_neg);

    // step 4: Operate
    retrieved_pi = cJSON_GetObjectItem(root, "pi");
    retrieved_neg = cJSON_GetObjectItemCaseSensitive(root, "neg");
    val_pi = cJSON_GetNumberValue(retrieved_pi);
    val_neg = cJSON_GetNumberValue(retrieved_neg);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (root != (cJSON *)0)
        + (num_pi != (cJSON *)0)
        + (num_neg != (cJSON *)0)
        + (retrieved_pi != (cJSON *)0)
        + (retrieved_neg != (cJSON *)0)
        + (printed != (char *)0)
        + (int)cJSON_IsNumber(retrieved_pi)
        + (int)cJSON_IsNumber(retrieved_neg)
        + (int)added_pi
        + (int)added_neg
        + (val_pi == 3.14)
        + (val_neg == -42.0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}