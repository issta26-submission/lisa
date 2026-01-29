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
//<ID> 1021
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "[1,2]";
    cJSON *root = (cJSON *)0;
    cJSON *nitem1 = (cJSON *)0;
    cJSON *nitem2 = (cJSON *)0;
    cJSON *fetched0 = (cJSON *)0;
    cJSON *fetched1 = (cJSON *)0;
    cJSON_bool is_arr = (cJSON_bool)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    double val_f0 = 0.0;
    double val_f1 = 0.0;
    int validation_score = 0;

    // step 2: Setup
    root = cJSON_Parse(json_text);

    // step 3: Configure
    nitem1 = cJSON_CreateNumber(3.5);
    nitem2 = cJSON_CreateNumber(4.5);
    add_ok1 = cJSON_AddItemToArray(root, nitem1);
    add_ok2 = cJSON_AddItemToArray(root, nitem2);

    // step 4: Operate
    is_arr = cJSON_IsArray(root);
    fetched0 = cJSON_GetArrayItem(root, 0);
    fetched1 = cJSON_GetArrayItem(root, 3);
    val_f0 = cJSON_GetNumberValue(fetched0);
    val_f1 = cJSON_GetNumberValue(fetched1);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0) + (int)(is_arr) + (int)(nitem1 != (cJSON *)0) + (int)(nitem2 != (cJSON *)0) + (int)(add_ok1) + (int)(add_ok2) + (int)(val_f0 == 1.0) + (int)(val_f1 == 4.5);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}