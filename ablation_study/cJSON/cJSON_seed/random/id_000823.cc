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
//<ID> 823
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_a = (cJSON *)0;
    cJSON *num_b = (cJSON *)0;
    cJSON *retr_a = (cJSON *)0;
    cJSON *retr_b = (cJSON *)0;
    cJSON_bool added_a = 0;
    cJSON_bool added_b = 0;
    cJSON_bool cmp = 0;
    double val = 0.0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_a = cJSON_CreateNumber(42.5);
    num_b = cJSON_CreateNumber(42.5);

    // step 3: Configure
    added_a = cJSON_AddItemToObject(root, "alpha", num_a);
    added_b = cJSON_AddItemToObject(root, "beta", num_b);

    // step 4: Operate
    retr_a = cJSON_GetObjectItem(root, "alpha");
    retr_b = cJSON_GetObjectItem(root, "beta");
    cmp = cJSON_Compare(retr_a, retr_b, 1);
    val = cJSON_GetNumberValue(retr_a);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)added_a + (int)added_b + (int)cmp + (int)(val == 42.5) + (printed != (char *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}