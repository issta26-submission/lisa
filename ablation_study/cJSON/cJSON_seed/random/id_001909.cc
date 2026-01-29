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
//<ID> 1909
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
    cJSON *added_age = NULL;
    cJSON *num_item = NULL;
    cJSON *got_age = NULL;
    cJSON *got_answer = NULL;
    cJSON *got_enabled = NULL;
    char *out = NULL;
    double age_val = 0.0;
    double answer_val = 0.0;
    cJSON_bool enabled_flag = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();

    // step 3: Core operations
    added_age = cJSON_AddNumberToObject(root, "age", 30.5);
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num_item);
    cJSON_AddBoolToObject(root, "enabled", 1);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    got_age = cJSON_GetObjectItem(root, "age");
    age_val = cJSON_GetNumberValue(got_age);
    got_answer = cJSON_GetObjectItem(root, "answer");
    answer_val = cJSON_GetNumberValue(got_answer);
    got_enabled = cJSON_GetObjectItem(root, "enabled");
    enabled_flag = cJSON_IsTrue(got_enabled);
    validation ^= (int)(root != NULL);
    validation ^= (int)(added_age != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(got_age != NULL);
    validation ^= (int)(got_answer != NULL);
    validation ^= (int)(got_enabled != NULL);
    validation ^= (int)(cJSON_IsNumber(got_age));
    validation ^= (int)(cJSON_IsNumber(got_answer));
    validation ^= (int)(age_val == 30.5);
    validation ^= (int)(answer_val == 42.0);
    validation ^= (int)(enabled_flag != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}