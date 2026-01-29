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
//<ID> 1927
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
    cJSON *age_added = NULL;
    cJSON *pi_created = NULL;
    cJSON *true_created = NULL;
    cJSON *got_age = NULL;
    cJSON *got_pi = NULL;
    cJSON *got_true = NULL;
    double age_val = 0.0;
    double pi_val = 0.0;
    cJSON_bool has_pi = 0;
    cJSON_bool is_true_flag = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    pi_created = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(root, "pi", pi_created);
    age_added = cJSON_AddNumberToObject(root, "age", 42.0);
    true_created = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_created);

    // step 3: Core operations
    got_age = cJSON_GetObjectItem(root, "age");
    got_pi = cJSON_GetObjectItem(root, "pi");
    got_true = cJSON_GetObjectItem(root, "enabled");
    age_val = cJSON_GetNumberValue(got_age);
    pi_val = cJSON_GetNumberValue(got_pi);
    has_pi = cJSON_HasObjectItem(root, "pi");
    is_true_flag = cJSON_IsTrue(got_true);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(age_added != NULL);
    validation ^= (int)(pi_created != NULL);
    validation ^= (int)(true_created != NULL);
    validation ^= (int)(got_age != NULL);
    validation ^= (int)(got_pi != NULL);
    validation ^= (int)(got_true != NULL);
    validation ^= (int)(age_val == 42.0);
    validation ^= (int)(pi_val == 3.1415);
    validation ^= (int)(has_pi != 0);
    validation ^= (int)(is_true_flag != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}