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
//<ID> 786
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *nested = (cJSON *)0;
    cJSON *num_int = (cJSON *)0;
    cJSON *num_pi = (cJSON *)0;
    cJSON *num_neg = (cJSON *)0;
    cJSON *retr_pi = (cJSON *)0;
    cJSON *retr_int_cs = (cJSON *)0;
    char *printed = (char *)0;
    double val_pi = 0.0;
    double val_int = 0.0;
    cJSON_bool is_num_pi = 0;
    cJSON_bool is_num_int = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    nested = cJSON_CreateObject();
    num_int = cJSON_CreateNumber(42.0);
    num_pi = cJSON_CreateNumber(3.1415);
    num_neg = cJSON_CreateNumber(-7.5);

    // step 3: Configure
    cJSON_AddItemToObject(root, "int_value", num_int);
    cJSON_AddItemToObject(root, "pi", num_pi);
    cJSON_AddItemToObject(root, "meta", nested);
    cJSON_AddItemToObject(nested, "neg", num_neg);

    // step 4: Operate
    retr_pi = cJSON_GetObjectItem(root, "pi");
    retr_int_cs = cJSON_GetObjectItemCaseSensitive(root, "int_value");
    val_pi = cJSON_GetNumberValue(retr_pi);
    val_int = cJSON_GetNumberValue(retr_int_cs);
    is_num_pi = cJSON_IsNumber(retr_pi);
    is_num_int = cJSON_IsNumber(retr_int_cs);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (retr_pi != (cJSON *)0) + (retr_int_cs != (cJSON *)0) + (printed != (char *)0) + (int)is_num_pi + (int)is_num_int + (val_pi != 0.0) + (val_int == 42.0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}