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
//<ID> 1606
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child_obj = (cJSON *)0;
    cJSON *num_pi = (cJSON *)0;
    cJSON *num_e = (cJSON *)0;
    cJSON *dup_root = (cJSON *)0;
    cJSON *dup_pi = (cJSON *)0;
    cJSON *dup_constants = (cJSON *)0;
    cJSON *dup_e = (cJSON *)0;
    double val_pi = 0.0;
    double val_e = 0.0;
    cJSON_bool added_pi = 0;
    cJSON_bool added_child_e = 0;
    cJSON_bool added_child_to_root = 0;
    cJSON_bool isobj_dup = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child_obj = cJSON_CreateObject();
    num_pi = cJSON_CreateNumber(3.141592653589793);
    num_e = cJSON_CreateNumber(2.718281828459045);

    // step 3: Configure
    added_child_e = cJSON_AddItemToObjectCS(child_obj, "e", num_e);
    added_child_to_root = cJSON_AddItemToObjectCS(root, "constants", child_obj);
    added_pi = cJSON_AddItemToObjectCS(root, "pi", num_pi);

    // step 4: Operate
    dup_root = cJSON_Duplicate(root, 1);
    isobj_dup = cJSON_IsObject(dup_root);
    dup_pi = cJSON_GetObjectItem(dup_root, "pi");
    dup_constants = cJSON_GetObjectItem(dup_root, "constants");
    dup_e = cJSON_GetObjectItem(dup_constants, "e");
    val_pi = cJSON_GetNumberValue(dup_pi);
    val_e = cJSON_GetNumberValue(dup_e);

    // step 5: Validate
    validation_score = (int)added_child_e
        + (int)added_child_to_root
        + (int)added_pi
        + (int)isobj_dup
        + (int)(dup_root != (cJSON *)0)
        + (int)(dup_pi != (cJSON *)0)
        + (int)(dup_constants != (cJSON *)0)
        + (int)(dup_e != (cJSON *)0)
        + (int)(val_pi > 3.0)
        + (int)(val_e > 2.0);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup_root);
    (void)validation_score;
    (void)val_pi;
    (void)val_e;
    (void)dup_pi;
    (void)dup_constants;
    (void)dup_e;
    // API sequence test completed successfully
    return 66;
}